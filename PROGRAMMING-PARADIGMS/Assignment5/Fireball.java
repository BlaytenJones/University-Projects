// Blayten K. Jones //
//    10/27/2022    //
//Projectile sprite that is released by Mario and kills enemies//

import java.awt.Graphics;
import java.awt.image.BufferedImage;

class Fireball extends Sprite {
    static BufferedImage fireballImage;
    boolean rightFacing = true;
    int existenceTimer = 0;
    int groundX = 0;
    boolean hitGround = false;
    
    Fireball(int x, int y, boolean right){
        super(x, y);
        w = fireballImage.getWidth(); h = fireballImage.getHeight();
        rightFacing = right;
        frames = 1;
    }

    Fireball(Json ob){
        super(ob);
        w = fireballImage.getWidth(); h = fireballImage.getHeight();
        frames = 1;
    }
    
    void draw(Graphics g, int scrollPos){
        g.drawImage(fireballImage, x - scrollPos, y, w, h, null);
    }

    void update(){
        //Counts how many cycles the fireball has existed.
        existenceTimer += 1;
        if(!hitGround){
            y += 39;
            if(y > ground - h){
                y = ground - h;
                //Stores the x at which it hit the ground so that we can offset the sin function
                groundX = x;
                hitGround = true;
            }
        }else{

            //Uses the graph of abs(sin(x)) as a basis for movement. I then stretched both the y and x to give a certain feel.
            //The -12 acts as an offset.
            y = ground - 30 + (int)(145.0 * -Math.abs((Math.sin(.025*(x - groundX)))));
        }
        //Moves the fireball left or right at a speed of 11.
        x += (rightFacing ? 1 : -1) * 11;
        //Removes the fireball after 116 cycles of existence (this allows the fireball to keep existing if the player is following it).
        remove = existenceTimer > 116;
    }

    void lazyLoadImage(){
        try{
            if(fireballImage == null){
                fireballImage = View.loadImage("fireball.png");
            }
		}catch(Exception e){
			e.printStackTrace(System.err);
			System.exit(1);
		}
    }

    @Override
    public String toString(){
        return "Fireball (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h + "; Frame = " + imageIndex;
    }
}
