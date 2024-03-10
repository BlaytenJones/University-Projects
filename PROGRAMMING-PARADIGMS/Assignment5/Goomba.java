// Blayten K. Jones //
//    10/27/2022    //
//Enemy sprite that moves back and forth//

import java.awt.image.BufferedImage;
import java.awt.Graphics;

class Goomba extends Sprite{
    int px, py;
    double vertVelocity;
    static BufferedImage[] animation = null;
    boolean flaming = false;
    boolean rightFacing = false;
    boolean isOnPipe = false;
    int deleteTimer = 0;

    Goomba(int x, int y){
        super(x, y);
        h = animation[0].getHeight(); w = animation[0].getWidth();
    }

    Goomba(Json ob){
        super(ob);
        h = animation[0].getHeight(); w = animation[0].getWidth();
        rightFacing = ob.getBool("rightFacing"); flaming = ob.getBool("flaming");
    }
    
    void draw(Graphics g, int scrollPos){
        //Creates goomba's sprite.
		//Uses a ternary operator to determine if the goomba is facing left. If they are, it will add on the width to the position
		//and make its width negative to effectively flip the image around. Also determines if the goomba is flaming and changes sprite
        //accordingly.
        g.drawImage(animation[flaming ? 1 : 0], x - scrollPos + (rightFacing ? 0 : w), y, (rightFacing ? 1 : -1) * w, h, null);
    }

    void update(){
        vertVelocity += 1.2;
        y += vertVelocity;

        if(y > ground-h){
			vertVelocity = 0.0;
			y = ground-h; // snap back to the ground
		}

        if(isOnPipe){
            vertVelocity = 0.0;
        }

        //moves the x left or right (depending on which direction it is facing) at a speed of 5. It will stop if it is on fire.
        x += (flaming ? 0 : 1) * (rightFacing ? 1 : -1) * 5;

        //Increments the delete counter if the goomba is currently on fire.
        deleteTimer += (flaming ? 1 : 0) * 1;

        //Sets the goomba for deletion after 30 cycles of being on fire.
        remove = (deleteTimer > 30);
        
        //In the case that it interacts with a pipe diagonally, the gravity will become slower as it is both in the state of being
        //"on the pipe" and falling. This line of code ensures that anytime the character is falling, it is not on the pipe.
        if(py != y){
            isOnPipe = false;
        }
    }

    void lazyLoadImage(){
        //Lazy loads the animation for the goomba; if one of the images is not found, it throws an error.
        if(animation == null){
            animation = new BufferedImage[2];
            frames = 1;
            try{
                animation[0] = View.loadImage("goomba.png");
                animation[1] = View.loadImage("goomba_fire.png");
            }catch(Exception e){
                System.out.println("Either the goomba image or the goomba_fire does not exist.");
                System.exit(1);
            }
        }
    }

    public void setPreviousPos(){
        px = x; py = y;
    }

    public void pushOutOfCollision(Sprite sprite){
        //Top collision.
        //Does not work with the standard height trick...
        if((py) <= sprite.y){
            y = sprite.y - h;
            isOnPipe = true;
            return;
        }

        //Left collision.
        if((px - w) < sprite.x){
            rightFacing = false;
            //It needs this or else it will teleport through the pipes. This is likely due to the fact that the width is actively changing
            px = sprite.x - sprite.w;
            x = px;
        }
        //Right collision.
        if(px > (sprite.x - sprite.w)){
            isOnPipe = false;
            x = sprite.x + sprite.w;
            rightFacing = true;
        }

        //no Bottom collision is needed as goombas don't jump
    }

    boolean existsHere(int x, int y){
        //Determines if a pipe exists at the position at these coordinates.
        if((x > this.x && x < (this.x + w)) && (y > this.y && y < (this.y + h))){
            return true;
        }else{
            return false;
        }
    }

    Json marshal(){
        Json ob = Json.newObject();
        ob.add("x", x); ob.add("y", y); ob.add("rightFacing", rightFacing); ob.add("flaming", flaming);
        return ob;
    }

    @Override
    public String toString(){
        return "Goomba (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h + "; Frame = " + imageIndex + "; Flaming = " + flaming;
    }
}
