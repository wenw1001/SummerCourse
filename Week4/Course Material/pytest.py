import cv2

if __name__ == '__main__':

	img = cv2.imread('images/cat.jpg')	
 
	cv2.imshow('cat', img)

	cv2.waitKey(0)
	cv2.destroyAllWindows()