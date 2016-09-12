include <buttons.scad>;

translate([-35/2,0,0]) difference() {
   buttonBase();
   linear_extrude(height=1) {
      eyes();
      translate([-9, -9, 0]) rotate([0,0,12]) square(size=[18,2]);
   }
}
