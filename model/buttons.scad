$fn=200;

module retainer(t=2) {
    cube([t,1,1]);
    translate([0,0,1]) cube([t, 3, 1]);
}
module buttonBase_v1(d=30, h=4) {
    cylinder(h=h,d=d);
    translate([0,0,h]) cylinder(d1=8, d2=4.5, h=2);
    translate([-d/2+1,-1,h]) rotate([0,0,90]) retainer();
    translate([d/2-1,1,h]) rotate([0,0,270]) retainer();
    translate([-1,d/2-1,h]) retainer();
    translate([1,-d/2+1,h]) rotate([0,0,180]) retainer();
}
module buttonBase_v2(d=30, h=4) {
    cylinder(h=h,d=d);
    translate([0,0,h]) cylinder(d1=8, d2=4.5, h=2);
    translate([0,0,h]) difference() {
       union() {
         cylinder(h=1, d=d);
         translate([0,0,1]) cylinder(h=1, d=d+2);
       }
       cylinder(h=2, d=d-2);
    }
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
