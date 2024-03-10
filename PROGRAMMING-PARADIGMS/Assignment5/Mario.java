// Blayten K. Jones //
//    10/13/2022    //
//Contains the player character's sprites and position//

import java.awt.image.BufferedImage;
import java.awt.Graphics;

class Mario extends Sprite{
    int px, py;
    double vertVelocity;
    boolean rightFacing = true;
    static BufferedImage[] animation = null;
    int jumpTimer;
    boolean isOnPipe = false;

    public Mario(int x, int y){
        super(x, y);
        jumpTimer = 0;
        h = animation[0].getHeight(); w = animation[0].getWidth();
    }

    void lazyLoadImage() {
        //Lazy loads the animation for the player; if one of the images is not found, it throws an error.
        if(animation == null){
            animation = new BufferedImage[5];
            frames = animation.length;
            for(int i = 0; i < animation.length; i++){
                try{
                    animation[i] = View.loadImage("mario" + (i + 1) + ".png");
                }catch(Exception e){
                    System.out.println("Player image " + (i + 1) + " does not exist.");
                    System.exit(1);
                }
            }
        }
    }

    void draw(Graphics g, int scrollPos){
        //Creates player's sprite.
		//Uses a ternary operator to determine if the player is facing left. If they are, it will add on the width to the position
		//and make its width negative to effectively flip the image around.
        g.drawImage(animation[imageIndex], x - scrollPos + (rightFacing ? 0 : w), y, (rightFacing ? 1 : -1) * w, h, null);
    }

    void update(){
        vertVelocity += 1.2;
        y += vertVelocity;
        jumpTimer++;

        if(y > ground-h){
			vertVelocity = 0.0;
			y = ground-h; // snap back to the ground
            jumpTimer = 0;
		}

        if(isOnPipe){
            vertVelocity = 0.0;
            jumpTimer = 0;
        }
        //In the case that it interacts with a pipe diagonally, the gravity will become slower as it is both in the state of being
        //"on the pipe" and falling. This line of code ensures that anytime the character is falling, it is not on the pipe.
        if(py != y){
            isOnPipe = false;
        }
    }

    public void pushOutOfCollision(Sprite sprite){
        //Top collision.
        if((py + h) <= sprite.y){
            isOnPipe = true;
            y = sprite.y - h;
            return;
        }

        isOnPipe = false;

        //Bottom collision.
        if(py >= (sprite.y + sprite.h)){
            //This part of the collision ensures that if the player "bonks" into the sprite, they will fall back down and have their
            //velocity eaten (this makes the collision feel more realistic).
            vertVelocity = 0;
            jumpTimer = 5;
            y = sprite.y + sprite.h;
            return;
        }

        //Left collision.
        if((px - w) < sprite.x){
            x = sprite.x - w;
        }
        
        //Right collision.
        if(px > (sprite.x - sprite.w)){
            x = sprite.x + sprite.w;
        }
    }

    public void setPreviousPos(){
        px = x; py = y;
    }

    @Override
    public String toString(){
        return "Mario (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h + "; Frame = " + imageIndex + "; Velocity y = " + vertVelocity;
    }
}