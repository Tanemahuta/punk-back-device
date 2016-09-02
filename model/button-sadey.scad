include <buttons.scad>;

difference() {
    buttonBase_v2();
    translate([0,0,0]) linear_extrude(height=1) {
            eyes();
 translate([0,-13, 0]) rotate([0,0,180]) round_mouth();
    }
}
