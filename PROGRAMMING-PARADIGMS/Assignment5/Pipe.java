// Blayten K. Jones //
//    9/28/2022     //
//Contains the position of each individual pipe//

import java.awt.image.BufferedImage;

import java.awt.Graphics;

public class Pipe extends Sprite {
    static BufferedImage pipeImage = null;
    
    Pipe(int x, int y){
        super(x, y);
        frames = 1;
        h = pipeImage.getHeight(); w = pipeImage.getWidth();
    }

    Pipe(Json ob){
        super(ob);
        h = pipeImage.getHeight(); w = pipeImage.getWidth();
    }

    void lazyLoadImage() {
        //Lazy loads the pipe image; if it is not found, it throws an error.
        try{
            if(pipeImage == null){
                pipeImage = View.loadImage("pipe.png");
            }
		}catch(Exception e){
			e.printStackTrace(System.err);
			System.exit(1);
		}
    }

    void draw(Graphics g, int scrollPos){
        g.drawImage(pipeImage, x - scrollPos, y, w, h, null);
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
        ob.add("x", x); ob.add("y", y);
        return ob;
    }

    @Override
    public String toString(){
        return "Pipe (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h;
    }

    void update(){
    }
}
