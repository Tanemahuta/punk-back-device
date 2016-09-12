include <buttons.scad>;

translate([35/2,0,0]) difference() {
   buttonBase();
   linear_extrude(height=1) {
      eyes();
      translate([0,0.5,0]) round_mouth();
   }
}
