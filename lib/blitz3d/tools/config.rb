module Blitz3D
  module Tools
    module Config
      PREMAKE_DIR = 'src/runtime/premake'
      STUB_DIR    = File.join(PREMAKE_DIR, 'stubs')
      LIB_DIR     = File.join(File.dirname(Blitz3D.path), '..', '_release', 'lib')

      def file_has_changed?(file, string)
        File.open(file).read != string
      rescue Errno::ENOENT
        true
      end

      def link_stub(mod)
        puts "Generating stub for #{mod.name.bold}..."
        f = StringIO.new
        f.write "// AUTOGENERATED. DO NOT EDIT.\n"
        f.write "// RUN `bin/blitz3d config` TO UPDATE.\n\n"
        f.write "#include <bb/blitz/module.h>\n"
        f.write "#include <bb/#{mod.id}/#{mod.id}.h>\n\n"
        f.write "#ifdef WIN32\n"
        f.write "BBMODULE_LINK( #{mod.id.gsub('.', '_')} ){\n"
        mod.symbols.each_pair do |ident, sym|
          f.write "\trtSym( \"#{ident}\",#{sym} );\n"
        end

        mod.commands.each do |command|
          params = command.params.map(&:to_rtsym)
          sym = "#{command.return_type}#{command.name}#{params.join('')}".inspect
          f.write "\trtSym( #{sym},#{command.symbol} );\n"
        end
        f.write "}\n"
        f.write "#endif\n"

        out_file = File.join(STUB_DIR, "#{mod.id}.cpp")

        if file_has_changed?(out_file, f.string)
          FileUtils.mkdir_p STUB_DIR
          File.open(out_file, 'w') { |h| h.write f.string }
        end
      end

      def premake(mod)
        FileUtils.mkdir_p PREMAKE_DIR

        puts "Generating premake file for #{mod.name.bold}..."
        File.open(File.join(PREMAKE_DIR, "#{mod.id}.lua"), 'w') do |f|
          f.write "-- AUTOGENERATED. DO NOT EDIT.\n"
          f.write "-- RUN `bin/blitz3d config` TO UPDATE.\n"
          f.write "project '#{mod.id}'\n"
          f.write "  kind 'StaticLib'\n"
          f.write "  language 'C++'\n\n"

          f.write "  filter { 'platforms:macos or linux or mingw32' }\n"
          f.write "    buildoptions { '-Wnon-virtual-dtor' }\n\n"
          # f.write "  filter { 'platforms:macos' }\n"
          # f.write "    buildoptions { '-Weverything', '-Wno-unused-parameter', '-Wno-sign-compare', '-Wno-sign-conversion', '-Wno-old-style-cast', '-Wno-unused-function', '-Wno-padded', '-Wno-double-promotion', '-Wshorten-64-to-32' }\n\n"

          f.write "  filter {}\n\n"

          inactive_platforms = Module::PLATFORMS.reject { |p| mod.platforms.empty? || mod.platforms.include?(p) }.map(&:inspect)
          unless inactive_platforms.empty?
            f.write "  removeplatforms { #{inactive_platforms.join(', ')} }\n\n"
          end

          unless mod.dependencies.nil? || mod.dependencies.empty?
            f.write "  links { "
            f.write mod.dependencies.map { |f| "'#{f.id}'" }.join(', ')
            f.write " }\n\n"
          end

          f.write "  files 'stubs/#{mod.id}.cpp'\n\n"

          options = [mod.premake5].flatten
          options.each do |option|
            option.each_pair do |key, value|
              case key
              when 'files'
                files = value.map { |path| File.join('..', '..', '..', mod.path, path) }

                f.write "  files { "
                f.write files.map { |f| "'#{f}'" }.join(', ')
                f.write " }\n"
              when 'links'
                deps = [value].flatten.map(&:inspect)
                f.write "  links { #{deps.join(', ')} }\n"
              when 'filter'
                filters = [value].flatten.map(&:inspect)
                f.write "  filter { #{filters.join(', ')} }\n"
              when 'includedirs'
                dirs = [value].flatten.map { |path| File.join('..', '..', '..', mod.path, path) }.map(&:inspect)
                f.write "  includedirs { #{dirs.join(', ')} }\n"
              else
                throw "unhandled key: '#{key}' in #{mod.name.bold}"
              end
            end
          end
        end
      end

      def run
        FileUtils.mkdir_p STUB_DIR
        FileUtils.mkdir_p LIB_DIR

        Runtime.all.each do |runtime|
          puts "Generating runtime #{runtime.name.bold}..."
          runtime.platforms.each do |platform|
            puts "  #{platform}..."
            modules = runtime.dependencies(:list, platform)

            config = {}
            config[:modules] = modules.map do |mod|
              mod.id
            end

            config[:commands] = modules.map do |mod|
              mod.commands.map do |command|
                {
                  name: command.name,
                  return_type: command.return_type,
                  symbol: command.symbol,
                  parameters: command.params.map do |param|
                    { ident: param.identifier, type: param.type, default: param.default }
                  end
                }
              end
            end.flatten

            File.open(File.join(LIB_DIR, "#{runtime.id}.#{platform}.config.json"), 'w') { |t| t.write JSON.pretty_generate(config) }

            f = StringIO.new
            f.write "// AUTOGENERATED. DO NOT EDIT.\n"
            f.write "// RUN `bin/blitz3d config` TO UPDATE.\n"
            f.write("\n#include <bb/stub/stub.h>\n\n")

            modules.each do |mod|
              f.write("BBMODULE_DECL( #{mod.id.parameterize.underscore} );\n")
            end

            f.write("#ifdef WIN32")
            f.write("\nvoid bbruntime_link( void (*link)( const char *sym,void *pc ) ){\n")
              modules.each do |mod|
                f.write("\t#{mod.id.parameterize.underscore}_link( link );\n")
              end
            f.write("}\n")
            f.write("#endif")

            f.write("\nbool bbruntime_create(){\n")
            write_create_calls = lambda do |i, l|
              (i + 1).times { f.write "\t" }
              id = modules[i].id.parameterize.underscore
              f.write("if( #{id}_create() ){\n")
              (i + 2).times { f.write "\t" }
              if i < modules.size - 1
                l.call(i + 1, l)
                (i + 1).times { f.write "\t" }
                f.write("\t#{id}_destroy();\n");
              else
                (i + 1).times { f.write "\t" }
                f.write("\treturn true;\n")
              end
              (i + 1).times { f.write "\t" }
              f.write("}else sue( \"#{id}_create failed\" );\n")
            end
            write_create_calls.call 0, write_create_calls
            f.write("\treturn false;\n")
            f.write("}\n")

            f.write("\nbool bbruntime_destroy(){\n")
            modules.reverse.each do |mod|
              f.write("\t#{mod.id.parameterize.underscore}_destroy();\n")
            end
            f.write("\treturn true;\n")
            f.write("}\n")

            runtime_stub_file = File.join(PREMAKE_DIR, "stubs/runtime.#{runtime.id}.#{platform}.cpp")

            if file_has_changed?(runtime_stub_file, f.string)
              File.open(runtime_stub_file, 'w') { |t| t.write f.string }
            end
          end

          File.open(File.join(PREMAKE_DIR, "runtime.#{runtime.id}.lua"), 'w') do |f|
            f.write "-- AUTOGENERATED. DO NOT EDIT.\n"
            f.write "-- RUN `bin/blitz3d config` TO UPDATE.\n"
            f.write "project 'runtime.#{runtime.id}'\n"
            f.write "  language 'C++'\n\n"

            f.write "  filter 'platforms:macos or linux'\n"
            f.write "    kind 'StaticLib'\n\n"

            f.write "  filter 'platforms:win32 or win64 or mingw32'\n"
            f.write "    kind 'SharedLib'\n"
            f.write "    targetdir '../../../_release/bin'\n"
            f.write "    targetprefix ''\n\n"

            f.write "    files {\n"
            f.write "      '../../../bbruntime_dll/bbruntime_dll.h',\n"
            f.write "      '../../../bbruntime_dll/bbruntime_dll.cpp',\n"
            f.write "      '../../../bbruntime_dll/bbruntime_dll.rc',\n"
            f.write "      '../../../bbruntime_dll/resource.h',\n"
            f.write "      '../../../bbruntime_dll/dpi.manifest'\n"
            f.write "    }\n\n"

            Module::PLATFORMS.each do |platform|
              f.write "  filter 'platforms:#{platform}'\n"
              f.write "  files \"stubs/runtime.#{runtime.id}.#{platform}.cpp\"\n\n"
            end

            f.write "  filter {}\n\n"
            f.write "  links 'stub'\n\n"

            runtime.dependencies(:list).reverse.each do |mod|
              f.write "  links #{mod.id.inspect}\n"
            end

            f.write "\n"
            runtime.premake5.each do |item|
              item.each_pair do |function, parameters|
                case function
                when 'links'
                  parameters = [parameters]
                  f.write "  links { #{parameters.flatten.map(&:inspect).join(', ')} }\n"
                when 'filter'
                  filters = [parameters].flatten.map(&:inspect)
                  f.write "\n  filter { #{filters.join(', ')} }\n"
                end
              end
            end
            # runtime.premake5['links'].each do |mod|
            #
            # end
          end
          # throw runtime.dependencies
        end


        File.open(File.join(PREMAKE_DIR, 'init.lua'), 'w') do |f|
          f.write "-- AUTOGENERATED. DO NOT EDIT.\n"
          f.write "-- RUN `bin/blitz3d config` TO UPDATE.\n\n"
          Module.all.each do |mod|
            premake mod
            link_stub mod
            f.write "require('./#{mod.id}')\n"
          end

          Runtime.all.each do |runtime|
            f.write "require('./runtime.#{runtime.id}')\n"
          end
        end
      end
    end
  end
end
