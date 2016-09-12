module hole() {
    translate([-7/2,0,0]) {
        translate([0, 0, 2]) cube([7,7,4]);
        translate([0, 0, 0]) cube([1, 7, 6]);
        translate([6, 0, 0]) cube([1, 7, 6]);
        translate([-1, 0, 0]) cube([1, 7, 3]);
        translate([7, 0, 0]) cube([1, 7, 3]);
    }
}
module buttonHolder() {
    translate([-(25*2+37*3)/2,-(2*2+7)/2,0]) difference() {
        cube([25*2+37*3,2*2+7,6]);
        for (i = [ 0 : 1 : 3]) {
            translate([25+37*i,2,0]) hole();
        }
    }
}

buttonHolder();
