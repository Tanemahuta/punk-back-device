include <buttons.scad>;

translate([-35*3/2,0,0]) difference() {
    buttonBase();
    translate([0,0,0]) linear_extrude(height=1) {
            eyes();
 translate([0,-13, 0]) rotate([0,0,180]) round_mouth();
    }
}
