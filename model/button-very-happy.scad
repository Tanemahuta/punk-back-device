include <buttons.scad>;

translate([35*3/2,0,0])  difference() {
   buttonBase();
   linear_extrude(height=1) {
      eyes();
      translate([-9, -5,0]) square(size=[18,2]);
      translate([0,0.5,0]) round_mouth();
   }
}
