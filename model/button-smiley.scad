include <buttons.scad>;

difference() {
   buttonBase_v2();
   linear_extrude(height=1) {
      eyes();
      translate([0,0.5,0]) round_mouth();
   }
}
