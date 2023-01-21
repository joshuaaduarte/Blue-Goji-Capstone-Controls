import cv2
import mediapipe as mp
import numpy as np
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

cap = cv2.VideoCapture(2)
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
            if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x:
                xcenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x)/2)
            elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x:
                xcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x)/2)
            else:
                xcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x

            # Y center calculations

            if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y:
                ycenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y)/2)
            elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y:
                ycenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y)/2)
            else: 
                landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y

            # Z center calculations
            if landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z > landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z:
                zcenter = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z +((landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z - landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z)/2)
            elif landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z < landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z:
                zcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z +((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z - landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/2)
            else: 
                zcenter = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z
                # Draw dot on the center of the hips
            centerCoordinate = xcenter, ycenter
            hipCenter = tuple(np.multiply(centerCoordinate, [640, 480]).astype(int))
            

            cv2.circle(image,hipCenter,10, (0,0,255), cv2.FILLED)

            # # Calculate distance between hip points
            # leftHipx = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x 
            # rightHipx = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x 
            # distancex = str(leftHipx - rightHipx)
            # #print(distancex)
            # leftHipz = landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z 
            # rightHipz = landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z 
            # distancez = str(leftHipz - rightHipz)
            # distancePosition = (75,75)
            # #cv2.putText(image, distancex, distancePosition, cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,0,255),2,cv2.LINE_AA)

            
            #Arrow Orientation
            
            #cv2.arrowedLine(image, (y, x), (y2, x2), (0,0,255), 3, 5, 0, 0.1)
            pointOne = (500, 150)
            
            # Determine direction where user is facing
            pointTwoX = 500 + (-100)*(-1/((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/(landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x))) 
            pointTwoY = 150 + (100)*(-1/((landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].z-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].z)/(landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x-landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x))) 

            pointTwo = (int(pointTwoX), int(pointTwoY))
            cv2.arrowedLine(image, pointOne, pointTwo, (0,0,255), 3, 5, 0, 0.1)

        except:
            pass
        
        # Render detections
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                                mp_drawing.DrawingSpec(color=(245,117,66), thickness=2, circle_radius=2), 
                                mp_drawing.DrawingSpec(color=(245,66,230), thickness=2, circle_radius=2) 
                                 )    
 

        
        #Camera Text
        camText = 'Camera'
        textPosition = (470, 265)
        cv2.putText(image, camText, textPosition, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255),2,cv2.LINE_AA)
        
        cv2.imshow('Mediapipe Feed', image)



        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
