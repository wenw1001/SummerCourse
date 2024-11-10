import cv2
image_left = cv2.imread('images/left.jpg')
image_right = cv2.imread('images/right.jpg')

image_left_rgb = cv2.cvtColor(image_left, cv2.COLOR_BGR2RGB)
image_right_rgb = cv2.cvtColor(image_right, cv2.COLOR_BGR2RGB)
left_gray = cv2.cvtColor(image_left, cv2.COLOR_BGR2GRAY)
right_gray = cv2.cvtColor(image_right, cv2.COLOR_BGR2GRAY)


sift = cv2.SIFT_create()
# SIFT only can use gray
kp_left, des_left = sift.detectAndCompute(left_gray,None)
kp_right,des_right = sift.detectAndCompute(right_gray,None)
 
feature_left=cv2.drawKeypoints(left_gray,kp_left,image_left)
feature_right=cv2.drawKeypoints(right_gray,kp_right,image_right)

#save feature image
cv2.imwrite('left_feature.jpg', feature_left)
cv2.imwrite('right_feature.jpg', feature_right)



# FLANN parameters
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
search_params = dict(checks=50)   # or pass empty dictionary
 
flann = cv2.FlannBasedMatcher(index_params,search_params)
 
flannKnn_matches = flann.knnMatch(des_left,des_right,k=2)
 
# Need to draw only good matches, so create a mask
flnnKnn_matchesMask = [[0,0] for i in range(len(flannKnn_matches))]
 
# ratio test as per Lowe's paper
for i,(m,n) in enumerate(flannKnn_matches):
    if m.distance < 0.7*n.distance:
        flnnKnn_matchesMask[i]=[1,0]
 
draw_params = dict(matchColor = (0,255,0),
                   singlePointColor = (255,0,0),
                   matchesMask = flnnKnn_matchesMask,
                   flags = cv2.DrawMatchesFlags_DEFAULT)
 
flnnKnn_match_image = cv2.drawMatchesKnn(left_gray,kp_left,right_gray,kp_right,flannKnn_matches,None,**draw_params)
 

#save feature matching image
cv2.imwrite('feature_matching.jpg', flnnKnn_match_image)

import numpy as np
# store all the good matches as per Lowe's ratio test.
flnnKnn_good = []
for m, n in flannKnn_matches:
    if m.distance < 0.7 * n.distance:
        flnnKnn_good.append(m)
        
MIN_MATCH_COUNT = 10
if len(flnnKnn_good)>MIN_MATCH_COUNT:
    src_pts = np.float32([ kp_left[m.queryIdx].pt for m in flnnKnn_good ]).reshape(-1,1,2)
    dst_pts = np.float32([ kp_right[m.trainIdx].pt for m in flnnKnn_good ]).reshape(-1,1,2)
 
    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC,5.0) # M: Homography Matrix
    matchesMask = mask.ravel().tolist()
 
    h,w = left_gray.shape
    pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
    dst = cv2.perspectiveTransform(pts,M) # 4 points
 
    polylines_right = cv2.polylines(right_gray,[np.int32(dst)],True,255,3, cv2.LINE_AA)
 
else:
    print( "Not enough matches are found - {}/{}".format(len(flnnKnn_good), MIN_MATCH_COUNT) )
    matchesMask = None

draw_params = dict(matchColor = (0,255,0), # draw matches in green color
                   singlePointColor = None,
                   matchesMask = matchesMask, # draw only inliers
                   flags = 2)
 
out_img = cv2.drawMatches(left_gray,kp_left,polylines_right,kp_right,flnnKnn_good,None,**draw_params)


h_r,w_r = right_gray.shape
cut_r = np.float32([ [0-w_r+1,0],[0-w_r+1,h_r-1],[0,h_r-1],[0,0] ]).reshape(-1,1,2) 
m_get = cv2.getPerspectiveTransform(dst, cut_r) 
result = cv2.warpPerspective(image_right_rgb, m_get, (-1000,h_r)) 


stitching_image= np.concatenate((image_left_rgb, result), axis=1) 
#save result image
cv2.imwrite('result.jpg',cv2.cvtColor(stitching_image, cv2.COLOR_RGB2BGR))