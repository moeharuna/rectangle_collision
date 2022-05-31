# rectangle_collision
Simple program that checks how a given polygon relates to a given line.
Should work for any convex polygon. Non-convex polygons should also work, but they are untested. 
Run `make` to build it.
# Input
`lines.txt` contains line segments. Each segment defined by two points.  
`polys.txt` ontains polygons. Each polygon defined by its vertexes.  
Vertexes should be inputted in order they connected. e.g. rectangle ABCD should be inputted as A-D-C-B or as A-B-C-D but not as A-C-D-B.
# Output
`result.txt` contains numbers on number per line.  
Each line correspond to relation between polygon and segment on the same line in files `lines.txt` `polys.txt`  
+ 0 - means that segment is inside of polygon.(both points inside)
+ 1 - means that segment is crossing polygon.(no points inside)
+ 2 - means that segemnt is partilly inside polygon(one point inside)
+ 3 - means that segemnt is not related to polygon(no points inside)
