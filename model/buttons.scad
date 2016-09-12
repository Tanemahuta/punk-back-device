$fn=200;

module retainer(t=2) {
    cube([t,1,1]);
    translate([0,0,1]) cube([t, 3, 1]);
}
module buttonBase(d=30, h=5,s_h=1,s_w=2) {
    difference() {
        union() {
            color("black") intersection() {
                cylinder(h=h+2, d=d+s_h*2);
                union() {
                    for (i = [0:1]) {
                        r = d/2+s_h;
                        rotate([0,0,i*90]) translate([-s_w/2,-r,0]) cube([2,r*2,h+2]);
                    }
                }
            }
            color("red") cylinder(h=h,d=d);
            color("blue") translate([0,0,h]) cylinder(h=2, d=d+s_h*2);            
        }

        color("blue") translate([0,0,h]) cylinder(h=3, d=d-2);
    }
    color("green") translate([0,0,h]) cylinder(d1=8, d2=4.5, h=2);
}

module eyes() {
  translate([7, 6,0]) circle(d=5);
  translate([-7,6,0]) circle(d=5);
}

module round_mouth() {  
  translate([0,-1,0]) difference() {
    circle(d=20);
    circle(d=16.5);
    translate([-15,-4,0]) square(size=[30,20]);
    translate([-18.8,-20,0]) square(size=[10,20]);
    translate([8.8,-20,0]) square(size=[10,20]);
  }
  translate([-8,-5,0]) circle(d=1.8);
  translate([+8,-5,0]) circle(d=1.8);
}
