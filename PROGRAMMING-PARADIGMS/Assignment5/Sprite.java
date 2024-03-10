// Blayten K. Jones //
//    10/27/2022    //
//Creates a parent class for all visuals//

import java.awt.Graphics;

abstract class Sprite{
    int x, y, w, h;
    final int ground = 550;
    int frames, imageIndex;
    boolean remove = false;

    Sprite(int x, int y){
        this.x = x; this.y = y;
        imageIndex = 0;
        lazyLoadImage();
    }

    Sprite(Json ob){
        x = (int)ob.getLong("x"); y = (int)ob.getLong("y");
        lazyLoadImage();
    }

    abstract void update();
    abstract void draw(Graphics g, int scrollPos);
    abstract void lazyLoadImage();

    //returns the "simple" name of the object which represents its ID
    String returnID(){
        return this.getClass().getSimpleName();
    }

    boolean collisionDetection(Sprite collider){
        int selfRight = x + w;
        int colliderRight = collider.x + collider.w;
        int selfBottom = y + h;
        int colliderBottom = collider.y + collider.h;
        //checks right collision.
        if(selfRight < collider.x){
            return false;
        }
        //checks left collision.
        if(x > colliderRight){
            return false;
        }
        //checks top collision.
        if(selfBottom < collider.y){
            return false;
        }
        //checks bottom collision.
        if(y > colliderBottom){
            return false;
        }
        return true;
    }

    void animate(){
        //Loops between the amount of frames an image has.
        imageIndex++;
        imageIndex %= frames;
    }
}