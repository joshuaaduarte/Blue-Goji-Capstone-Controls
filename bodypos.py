import cv2
import mediapipe as mp
import numpy as np
import math

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose


def angle_of_normal_vector(point1, point2):
    x1, y1 = point1
    x2, y2 = point2
    # calculate the slope of the line formed by the two points
    slope = (y2 - y1) / (x2 - x1)
    # calculate the angle of the normal vector
    # angle = math.atan(slope)
    # if angle < 0:
    #     angle += 2 * math.pi
    angle = math.atan2(y2 - y1, x2 - x1) + math.pi/2
    angle = angle % (2*math.pi)
    
    return angle

def generate_line_equation(point, angle, length):
    x0, y0 = point
    # calculate the end point of the line
    x1 = x0 + length * math.cos(angle)
    y1 = y0 + length * math.sin(angle)
    # calculate the line equation in the form y = mx + b
    m = math.tan(angle)
    b = y0 - m * x0
    return m, b, (x0, y0), (int(x1), int(y1))

def find_centerpointX(left, right):
    if left > right:
        xcenter = right + ((left - right)/2)
    elif left < right:
        xcenter = left + ((right - left)/2)
    else: 
        xcenter = left
        
    return xcenter

def find_centerpointY(left, right):
    if left > right:
        ycenter = right + ((left - right)/2)
    elif left < right:
        ycenter = left + ((right - left)/2)
    else: 
        ycenter = left
        
    return ycenter

def find_centerpointZ(left, right):
    if left > right:
        zcenter = right + ((left - right)/2)
    elif left < right:
        zcenter = left + ((right - left)/2)
    else: 
        zcenter = left
        
    return zcenter

cap = cv2.VideoCapture(0)
## Setup mediapipe instance
with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose:
    while cap.isOpened():
        ret, frame = cap.read()
        
        # Recolor image to RGB
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False
      
        # Make detection
        results = pose.process(image)
    
        # Recolor back to BGR
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        
        # Extract landmarks
        try:
            landmarks = results.pose_landmarks.landmark
            #print(landmarks)  
                
            # Middle of Hip Dot
        
            # X Center Calculations
            xcenter = find_centerpointX(landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x, landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x)
            
            # Y Center Calculations
            ycenter = find_centerpointX(landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y, landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y)
            
            # Y center calculations



            # Draw dot on the center of the hips
            centerCoordinate = xcenter, ycenter
            hipCenter = tuple(np.multiply(centerCoordinate, [640, 480]).astype(int))
            
            cv2.circle(image,hipCenter,10, (0,0,255), cv2.FILLED)


            #Create Arrow and Orientation that is respective of hip positions
            pointOne = (landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x, landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)
            pointTwo = (landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x, landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z)
            
            angle = angle_of_normal_vector(pointOne, pointTwo)
            length = 100
            start_point = (500, 150)
            m, b, start_point, end_point = generate_line_equation(start_point, angle, length)

            plotStart = (500,150)
            
            cv2.arrowedLine(image, plotStart, end_point, (0,0,255), 3, 5, 0, 0.1)

        except:
            pass
        
        # Render detections
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                                mp_drawing.DrawingSpec(color=(245,117,66), thickness=2, circle_radius=2), 
                                mp_drawing.DrawingSpec(color=(245,66,230), thickness=2, circle_radius=2) 
                                 )    
 

        
        #Camera Text
        camText = 'Camera'
        textPosition = (470, 35)
        cv2.putText(image, camText, textPosition, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255),2,cv2.LINE_AA)
        
        cv2.imshow('Mediapipe Feed', image)



        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    
# Graveyard

            # Middle of Hip Dot
        
            # X Center Calculations
            # if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x:
            #     xcenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x)/2)
            # elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x:
            #     xcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x)/2)
            # else:
            #     xcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x
            
            # Y center calculations
            # if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y:
            #     ycenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y)/2)
            # elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y:
            #     ycenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y)/2)
            # else: 
            #     landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y
            
            # Z center calculations
            # if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z:
            #     zcenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z)/2)
            # elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z:
            #     zcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/2)
            # else: 
            #     zcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z

            ##########
            
            # Calculate distance between hip points
            
            # leftHipx = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x 
            # rightHipx = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x 
            # distancex = str(leftHipx - rightHipx)
            # #print(distancex)
            # leftHipz = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z 
            # rightHipz = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z 
            # distancez = str(leftHipz - rightHipz)
            # distancePosition = (75,75)
            # #cv2.putText(image, distancex, distancePosition, cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,0,255),2,cv2.LINE_AA)
            
            
            #cv2.arrowedLine(image, (y, x), (y2, x2), (0,0,255), 3, 5, 0, 0.1)
            #print(start_point)
            #start_point = int(start_point)
            #end_point = int(end_point)
            #start_point(500,150)
            
            # endPoint = (400, 170)
            # print(plotStart)
            # print(end_point)
            #print(type(endPoint))

            # Determine direction where user is facing
            #pointTwoX = 500 + (-100)*(-1/((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/(landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x))) 
            #pointTwoY = 150 + (100)*(-1/((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/(landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x))) 

            #pointTwo = (int(pointTwoX), int(pointTwoY))