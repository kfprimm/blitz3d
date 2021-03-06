# Sin# ( degrees# )

## Parameters

- degrees# = angle in degrees.

## Description

Sine of an angle. The angle is measured in degrees.
For angles between 0 and 90 degrees this is defined by the sides of a right triangle. The sine is the side opposite the angle divided by the hypotenuse.
Outside of 0 to 90 the definition uses a circle with radius=1. The angle is placed at the center of the circle, with one side on the positive x-axis. The other side hits the circle at some point. The y coordinate of this point is the sine of the angle.
The positive y-axis corresonds to +90 degrees. This is a common source of confusion in Blitz. With screen coordinates ( pixels ) the y-axis points downward. But in the 3d world the y-axis typically points upward.
Another possible snag is the size of the angle. In principle, the sine function repeats every 360 degrees. So Sin(-360), Sin(0), Sin(360), Sin(720) etc. should all be exactly the same. But in practice the accuracy decreases as the angle gets farther away from zero.
See also ASin, Cos, ACos, Tan, Atan, ATan2
