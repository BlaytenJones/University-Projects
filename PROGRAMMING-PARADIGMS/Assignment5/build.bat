::@echo off
javac Game.java View.java Controller.java Model.java Pipe.java Mario.java Json.java Sprite.java Goomba.java Fireball.java
if %errorlevel% neq 0 (
	echo There was an error; exiting now.	
) else (
	echo Compiled correctly!  Running Game...
	java Game	
)