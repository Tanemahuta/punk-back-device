include <buttons.scad>;
$w=25*2+37*3+2*2+1;
$h=50;
$t=20;
module hull() {
        union() {
            translate([4,4,0]) cube([$w-8,$h-8,$t]);
            translate([2,2,0]) cube([$w-4,$h-4,$t-2]);
            translate([0,0,$t-4]) {
                translate([4,4,0]) sphere(r=4);
                translate([$w-4,4,0]) sphere(r=4);
                translate([4,$h-4,0]) sphere(r=4);
                translate([$w-4,$h-4,0]) sphere(r=4);
                translate([4,4,0]) rotate([0,90,0]) cylinder(r=4,h=$w-8);
                translate([4,$h-4,0]) rotate([0,90,0]) cylinder(r=4,h=$w-8);
                translate([4,($h-8)+4,0]) rotate([90,90,0]) cylinder(r=4,h=$h-8);
                translate([$w-4,($h-8)+4,0]) rotate([90,90,0]) cylinder(r=4,h=$h-8);
            }
            translate([4,0,0]) cube([$w-8,4,$t-4]);
            translate([4,4,0]) cylinder(r=4,h=$t-4);
            translate([4,$h-4,0]) cube([$w-8,4,$t-4]);
            translate([$w-4,4,0]) cylinder(r=4,h=$t-4);
            translate([0,4,0]) cube([4,$h-8,$t-2]);
            translate([4,$h-4,0]) cylinder(r=4,h=$t-4);
            translate([$w-4,4,0]) cube([4,$h-8,$t-2]);
            translate([$w-4,$h-4,0]) cylinder(r=4,h=$t-2);
        }
    }

module clip() {
    translate([5, 2+11+2+1,2.5]) rotate([90,0,270]) linear_extrude(height=5)  polygon([[0,0],[0,9],[1,9],[3,7],[2,7],[2,0]]);
translate([0,0,2.5]) rotate([90,0,90]) linear_extrude(height=5)  polygon([[0,0],[0,9.5],[1,9.5],[3,7.5],[2,7.5],[2,0]]);
    translate([0,0,0]) cube([5,11+4+1,2.5]);
}

module allHull() {
    rotate([0,0,0]) translate([-$w/2,-$h/2,0]) {
        difference() {
            union() {
                difference() {
                    hull();
                    translate([2,2,-2]) cube([$w-4,$h-4,$t]);
                    translate([10,1,2]) cube([2,2,1]);
                    translate([$w/2-1,1,2]) cube([2,2,1]);
                    translate([$w-10,1,2]) cube([2,2,1]);
                    translate([10,$h-2,2]) cube([2,2,1]);
                    translate([$w/2-1,$h-2,2]) cube([2,1,1]);
                    translate([$w-10,$h-2,2]) cube([2,1,1]);
                }
                translate([4,$h/2+11/2+2+0.5,$t-2]) rotate([180,0,0]) clip();
                translate([$w-4-6,$h/2+11/2+2+0.5,$t-2]) rotate([180,0,0]) clip();    
                for (i = [0:2]) {
                    translate([2+25+37/2+37*i-1.5,$h/2+11/2+2+0.5,$t-2]) rotate([180,0,0]) scale([3/5,1,1]) clip();
                }
            }
            for (i = [0 : 3]) {
                translate([2+25+37*i, $h/2,$t-5]) 
                buttonBase(d=32, h=8,s_h=2,s_w=3);
            }
        }
    }
}
allHull();
