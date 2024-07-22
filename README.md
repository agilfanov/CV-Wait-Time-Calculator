# CV-Wait-Time-Calculator
Provides a system to calculate wait times for lines using a single camera setup.

Description:

 	Provides a way to count the number of people in line and deduce the current wait time, given line parameters. 
  	Goal is to provide quickly updating and accurate wait times for amusement parks or any other places with a line.


 Set Up Requirements:

 	OpenCV library: https://opencv.org/releases/

  	Tune the parameters as desired. 

  Physical Set Up:

  	Configure the camera to be facing directly down above the entrance to the line(bird's eye view of the people). Ensure it the camera has adequate space between itself and the people walking into the line. Also, ensure that the only field of view visible by the camera is the line area (this can also be done by cropping the camera's view with opencv). Then orient it such that when the camera displays the white line, the line is to the left of the white line. 

   Demo Pictures:

 	For the wait time, the parameters used are a 1 person cycle with a 2 min cycle time.

![image](https://github.com/user-attachments/assets/4fb4ef54-395d-4f4d-a511-99cd4c315cb5)


![image](https://github.com/user-attachments/assets/40927edb-f8cc-4312-b8c1-c71873b90db8)




https://github.com/user-attachments/assets/50d9b12b-10e5-45f3-9825-0b518bb13148

