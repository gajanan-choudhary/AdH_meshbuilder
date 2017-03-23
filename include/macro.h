/******************************************************************************************************************************************************************************************************/
// Generic macros

#define max2(a, b) (a>b?a:b);
#define min2(a, b) (a<b?a:b);
#define max3(a,b,c) (max2(max2(a,b),c);
#define min3(a,b,c) (min2(min2(a,b),c);
#define fabs(a) (a>0?a:-a);

#define throw_error(s) {printf("\n\n"); printf(s); printf("\n\n"); \ exit(-1);}

/******************************************************************************************************************************************************************************************************/
// Coordinate Geometry
#define VECT2D_DOT(v1,v2) (v1.x*v2.x + v1.y*v2.y);
#define VECT3D_DOT(v1,v2) (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
#define VECT2D_CROSS(v1,v2) (v1.x*v2.y - v2.x*v1.y);
#define VECT3D_CROSS(v1,v2) ((v1.x*v2.y + v1.y*v2.z + v1.z*v2.x) - (v2.x*v1.y + v2.y*v1.z + v2.z*v1.x));
#define BOX_PRODUCT(v1,v2,v3) VECT3D_DOT(v1,VECT3D_CROSS(v2,v3));
#define VECT2D_MAG(vect) sqrt(VECT2D_DOT(vect,vect));
#define VECT3D_MAG(vect) sqrt(VECT3D_DOT(vect,vect));

#define PROJECTED_DISTANCE(x1,y1,x2,y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
#define PROJECTED_AREA(x1,y1,x2,y2,x3,y3) ((x1*y2+x2*y3+x3*y1)-(x2*y1+x3*y2+x1*y3))/2.0;
#define VECT2D_AREA(v1,v2)  VECT2D_CROSS(v1,v2)/2.0;

/******************************************************************************************************************************************************************************************************/
// 2D Element macros

#define ELEM2D_AREA(e) PROJECTED_AREA(e.coord[0].x, e.coord[0].y, e.coord[1].x, e.coord[1].y, e.coord[2].x, e.coord[2].y);
#define AREA(xyz, n1, n2, n3) PROJECTED_AREA(xyz[n1].x, xyz[n1].y, xyz[n2].x, xyz[n2].y, xyz[n3].x, xyz[n3].y);

//#define ELEM2D_AREA(e) ((e.coord[0].x*e.coord[1].y + e.coord[1].x*e.coord[2].y + e.coord[2].x*e.coord[0].y) - (e.coord[1].x*e.coord[0].y + e.coord[2].x*e.coord[1].y + e.coord[0].x*e.coord[2].y))/2.0;
//#define AREA(x, n1, n2, n3) ((xyz[n1].x*xyz[n2].y + xyz[n2].x*xyz[n3].y + xyz[n3].x*xyz[n1].y) - (xyz[n2].x*xyz[n1].y + xyz[n3].x*xyz[n2].y + xyz[n1].x*xyz[n3].y))/2.0;

#define ELEM2D_STORE_NODE_COORDS(e, coord) \
    e.coord[0].x = coord[e.nodes[0]].x; \
    e.coord[0].y = coord[e.nodes[0]].y; \
                                        \
    e.coord[1].x = coord[e.nodes[1]].x; \
    e.coord[1].y = coord[e.nodes[1]].y; \
                                        \
    e.coord[2].x = coord[e.nodes[2]].x; \
    e.coord[2].y = coord[e.nodes[2]].y;

#define CREATE_ELEM2D(e, xyz, node0, node1, node2) \
    e.area = PROJECTED_AREA(xyz,node0,node1,node2); \
    if (e.area > 0) { \
        e.nodes[0] = node0; \
        e.nodes[1] = node1; \
        e.nodes[2] = node2; \
    } \
    else{ \
        e.nodes[0] = node0; \
        e.nodes[1] = node2; \
        e.nodes[2] = node1; \
        e.area = -e.area; \
    } \
    ELEM2D_STORE_NODE_COORDS(e, xyz);

/******************************************************************************************************************************************************************************************************/
// 1D Element Macros
//
#define ELEM1D_LENGTH(e) PROJECTED_DISTANCE(e.coord[0].x, e.coord[0].y, e.coord[1].x, e.coord[1].y);
#define LENGTH(xyz, n1, n2) PROJECTED_DISTANCE(xyz[n1].x, xyz[n1].y, xyz[n2].x, xyz[n2].y);

//#define ELEM1D_LENGTH(e) sqrt((e.coord[0].x-e.coord[1].x)*(e.coord[0].x-e.coord[1].x) + (e.coord[0].y-e.coord[1].y)*(e.coord[0].y-e.coord[1].y));
//#define LENGTH(x, n1, n2) sqrt((xyz[n1].x-xyz[n2].x)*(xyz[n1].x-xyz[n2].x) + (xyz[n1].y-xyz[n2].y)*(xyz[n1].y-xyz[n2].y));

#define ELEM1D_STORE_NODE_COORDS(e, coord) \
    e.coord[0].x = coord[e.nodes[0]].x; \
    e.coord[0].y = coord[e.nodes[0]].y; \
                                        \
    e.coord[1].x = coord[e.nodes[1]].x; \
    e.coord[1].y = coord[e.nodes[1]].y;

#define CREATE_ELEM1D(e, xyz, node0, node1) \
    e.nodes[0] = node0; \
    e.nodes[1] = node1; \
    ELEM1D_STORE_NODE_COORDS(e, xyz); \
    e.length = ELEM1D_LENGTH(e);

/******************************************************************************************************************************************************************************************************/
// Shape Function macros

#define EVAL_SHP_FNCTN_BASIC(ksi, eta, p1, p2, p3, p4)  (p1*(1-ksi)*(1-eta) + p2*(1-ksi)*(1+eta) + p3*(1+ksi)*(1+eta) + p4*(1+ksi)*(1-eta))/4.0;

#define EVAL_SHP_FNCTN(ksi, eta, values)  (values[0]*(1-ksi)*(1-eta) + values[1]*(1-ksi)*(1+eta) + values[2]*(1+ksi)*(1+eta) + values[3]*(1+ksi)*(1-eta))/4.0;

#define VECT2D_EVAL_SHP_FNCTN(ans, ksi, eta, corners) \
    ans.x = (corners[0].x*(1-ksi)*(1-eta) + corners[1].x*(1-ksi)*(1+eta) + corners[2].x*(1+ksi)*(1+eta) + corners[3].x*(1+ksi)*(1-eta))/4.0; \
    ans.y = (corners[0].y*(1-ksi)*(1-eta) + corners[1].y*(1-ksi)*(1+eta) + corners[2].y*(1+ksi)*(1+eta) + corners[3].y*(1+ksi)*(1-eta))/4.0;

#define VECT3D_EVAL_SHP_FNCTN(ans, ksi, eta, corners) \
    ans.x = (corners[0].x*(1-ksi)*(1-eta) + corners[1].x*(1-ksi)*(1+eta) + corners[2].x*(1+ksi)*(1+eta) + corners[3].x*(1+ksi)*(1-eta))/4.0; \
    ans.y = (corners[0].y*(1-ksi)*(1-eta) + corners[1].y*(1-ksi)*(1+eta) + corners[2].y*(1+ksi)*(1+eta) + corners[3].y*(1+ksi)*(1-eta))/4.0; \
    ans.z = (corners[0].z*(1-ksi)*(1-eta) + corners[1].z*(1-ksi)*(1+eta) + corners[2].z*(1+ksi)*(1+eta) + corners[3].z*(1+ksi)*(1-eta))/4.0;

/******************************************************************************************************************************************************************************************************/
