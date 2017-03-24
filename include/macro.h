#ifndef _MACROS
#define _MACROS
/******************************************************************************************************************************************************************************************************/
// Generic macros

#define max2(a, b)  ( ((a)>(b)) ? (a) : (b) )
#define min2(a, b)  ( ((a)<(b)) ? (a) : (b) )
//#define max3(a,b,c) (max2(max2(a,b),c) /* Something's wrong */
//#define min3(a,b,c) (min2(min2(a,b),c) /* Something's wrong */
#define max4(a,b,c,d) max2(max2(a,b),max2(c,d))
#define min4(a,b,c,d) min2(min2(a,b),min2(c,d))

#define throw_error(s) {printf("\n\n"); printf(s); printf("\n\n"); exit(-1);}
#define checkmem(ptr) \
    if (ptr == NULL){ \
        throw_error("Error: Could not allocate memory properly"); \
    }
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
#define TRIANGLE_AREA(xy,n1,n2,n3) ((xy[n1].x*xy[n2].y+xy[n2].x*xy[n3].y+xy[n3].x*xy[n1].y)-(xy[n2].x*xy[n1].y+xy[n3].x*xy[n2].y+xy[n1].x*xy[n3].y))/2.0;
#define VECT2D_AREA(v1,v2)  VECT2D_CROSS(v1,v2)/2.0;

/******************************************************************************************************************************************************************************************************/
// 2D Element macros

#define ELEM2D_AREA(e) PROJECTED_AREA(e.coord[0].x, e.coord[0].y, e.coord[1].x, e.coord[1].y, e.coord[2].x, e.coord[2].y);
#define AREA(xyz, n1, n2, n3) PROJECTED_AREA(xyz[n1].x, xyz[n1].y, xyz[n2].x, xyz[n2].y, xyz[n3].x, xyz[n3].y);

//#define ELEM2D_AREA(e) ((e.coord[0].x*e.coord[1].y + e.coord[1].x*e.coord[2].y + e.coord[2].x*e.coord[0].y) - (e.coord[1].x*e.coord[0].y + e.coord[2].x*e.coord[1].y + e.coord[0].x*e.coord[2].y))/2.0;
//#define AREA(x, n1, n2, n3) ((xyz[n1].x*xyz[n2].y + xyz[n2].x*xyz[n3].y + xyz[n3].x*xyz[n1].y) - (xyz[n2].x*xyz[n1].y + xyz[n3].x*xyz[n2].y + xyz[n1].x*xyz[n3].y))/2.0;

#define leftmostx(nodes)    min4(nodes[0].x, nodes[1].x, nodes[2].x, nodes[3].x)
#define rightmostx(nodes)   max4(nodes[0].x, nodes[1].x, nodes[2].x, nodes[3].x)
#define bottommosty(nodes)  min4(nodes[0].y, nodes[1].y, nodes[2].y, nodes[3].y)
#define topmosty(nodes)     max4(nodes[0].y, nodes[1].y, nodes[2].y, nodes[3].y)
// #define CREATE_ELEM2D(e, j, nrows, i, ncols, xyz, nodes)                                       
//     nodes[0] = i*nrows+j;                                                                      
//     nodes[1] = nodes[0]+1;                                                                     
//     nodes[2] = nodes[0]+nrows;                                                                 
//     nodes[3] = nodes[2]+1;                                                                     
//     /* We know that nodes [0] is diagonally opposite to nodes 4 by above definition. */        
//     if (xyz[nodes[0]].x < xyz[nodes[3]].x) /* node[0] is on the left, node[3] on the right */  
//         if(xyz[nodes[0]].y < xyz[nodes[3].y]){                                                 
//             /* node[0] is bottom-left, node[3] is top-right*/                                  
//         }                                                                                      
//         else{                                                                                  
//             /* node[0] is top-left, node[3] is bottom-right */                                 
//         }                                                                                      
//     }                                                                                          
//     else{ /* node[0] is on the right, node[3] on the left */                                   
//         if(xyz[nodes[0]].y < xyz[nodes[3].y]){                                                 
//             /* node[0] is bottom-right, node[3] is top-left*/                                  
//         }                                                                                      
//         else{                                                                                  
//             /* node[0] is top-right, node[3] is bottom-left */                                 
//         }                                                                                      
//     }                                                                                          
    

#define ELEM2D_STORE_NODE_COORDS(e, xyz) \
    e.coord[0].x = xyz[e.nodes[0]].x; \
    e.coord[0].y = xyz[e.nodes[0]].y; \
                                      \
    e.coord[1].x = xyz[e.nodes[1]].x; \
    e.coord[1].y = xyz[e.nodes[1]].y; \
                                      \
    e.coord[2].x = xyz[e.nodes[2]].x; \
    e.coord[2].y = xyz[e.nodes[2]].y;

#define CREATE_ELEM2D(e, xyz, node0, node1, node2) \
    e.area = TRIANGLE_AREA(xyz,node0,node1,node2); \
    if (e.area > 0) { \
        e.nodes[0] = node0; \
        e.nodes[1] = node1; \
        e.nodes[2] = node2; \
    } \
    else{ \
        e.nodes[0] = node2; \
        e.nodes[1] = node1; \
        e.nodes[2] = node0; \
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
#endif
