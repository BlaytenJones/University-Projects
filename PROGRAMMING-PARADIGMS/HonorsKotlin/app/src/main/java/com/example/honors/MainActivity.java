package com.example.honors;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity
{
    Model model;
    static GameView view;
    GameController controller;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        model = new Model(this);
        controller = new GameController(model, view);
        setContentView(view);
        this.model.addSprite(974, 381, "Pipe"); this.model.addSprite(1156, 310, "Pipe"); this.model.addSprite(1598, 498, "Pipe"); this.model.addSprite(533, 380, "Pipe"); this.model.addSprite(-90, 502, "Pipe"); this.model.addSprite(351, 459, "Pipe"); this.model.addSprite(1250, 550, "Goomba"); this.model.addSprite(1300, 450, "Goomba"); this.model.addSprite(700, 450, "Goomba");
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState)
    {
        super.onPostCreate(savedInstanceState);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        controller.resume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        controller.pause();
    }



    static class Model
    {
        ArrayList<Sprite> sprites;
        Mario player;

        Model(Context context){
            view = new GameView(context, this);
            sprites = new ArrayList<>();
            player = new Mario(100, 100, view);
            sprites.add(player);
        }

        //NOTE: updates the position of all sprites and checks for general collisions
        public void update(){
            boolean check = false;
            for(int i = 0; i < sprites.size(); i++){
                Sprite collidee = sprites.get(i);
                collidee.update();
                //Removes the sprite if the remove variable is true
                if(collidee.remove){
                    sprites.remove(i);
                }
                //Checks every sprite compared to a different sprite for collision
                for(int j = 0; j < sprites.size(); j++){
                    Sprite collider = sprites.get(j);
                    //If the sprite colliding with something is not a pipe (as pipes do not move) then check to see that it is not
                    //the same sprite as itself. Finally, it determines if the sprite is overlapping with a different sprite.
                    if((collider.returnID() != "Pipe") && (i != j) && (collider.collisionDetection(collidee))){
                        check = true;
                        //I really tried my best to do something elegant. Have this code instead.
                        if(collider.returnID() == "Mario"){
                            if(collidee.returnID() == "Pipe"){
                                ((Mario)collider).pushOutOfCollision((Pipe)collidee);
                            }
                        }else if(collider.returnID() == "Goomba"){
                            if(collidee.returnID() == "Pipe"){
                                ((Goomba)collider).pushOutOfCollision((Pipe)collidee);
                            } else if(collidee.returnID() == "Fireball"){
                                //Sets the Goomba on fire if it collides with fireball.
                                ((Goomba)collider).flaming = true;
                                sprites.remove(i);
                                break;
                            }
                        }
                    }
                }
            }
            if(!check){
                player.isOnPipe = false;
                for(int i = 0; i < sprites.size(); i++){
                    if(sprites.get(i).returnID() == "Goomba"){
                        ((Goomba)sprites.get(i)).isOnPipe = false;
                    }
                }
            }
        }

        //NOTE: sets the sprite's position when given an x and y value
        //It will then use a certain function depending on its ID.
        public void addSprite(int x, int y, String ID){
            switch(ID){
                case "Goomba":
                    sprites.add(new Goomba(x, y, view));
                    break;
                case "Fireball":
                    sprites.add(new Fireball(x, y, view, player.rightFacing));
                    break;
                case "Pipe":
                    sprites.add(new Pipe(x, y, view));
                    break;
            }
        }
    }




    static class GameView extends SurfaceView
    {
        SurfaceHolder ourHolder;
        static Canvas canvas;
        Paint paint;
        Model model;
        GameController controller;
        int scrollPos;

        public GameView(Context context, Model m)
        {
            super(context);
            model = m;

            // Initialize ourHolder and paint objects
            ourHolder = getHolder();
            paint = new Paint();
            scrollPos = 0;
        }

        void setController(GameController c)
        {
            controller = c;
        }

        public void update()
        {
            scrollPos = model.player.x - 100;

            if (!ourHolder.getSurface().isValid())
                return;
            canvas = ourHolder.lockCanvas();

            // Draw the background color
            canvas.drawColor(Color.argb(255, 128, 200, 200));

            for(int i = 0; i < model.sprites.size(); i++){
                model.sprites.get(i).draw(canvas, scrollPos);
            }

            paint.setColor(Color.rgb(255, 165, 0));
            paint.setStrokeWidth(10);
            canvas.drawRect(0, 1050, 2000, 3050, paint);

            ourHolder.unlockCanvasAndPost(canvas);
        }

        // The SurfaceView class (which GameView extends) already
        // implements onTouchListener, so we override this method
        // and pass the event to the controller.
        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouchEvent(MotionEvent motionEvent)
        {
            controller.onTouchEvent(motionEvent);
            return true;
        }
    }




    static class GameController implements Runnable
    {
        volatile boolean playing;
        Thread gameThread = null;
        Model model;
        GameView view;
        boolean Left;
        boolean Right;
        boolean Jump;
        boolean Fire;
        private int step = 6;

        GameController(Model m, GameView v)
        {
            model = m;
            view = v;
            view.setController(this);
            playing = true;
        }

        void update()
        {
            model.player.setPreviousPos();
            for(int i = 0; i < model.sprites.size(); i++){
                if(model.sprites.get(i).returnID() == "Goomba"){
                    ((Goomba)model.sprites.get(i)).setPreviousPos();
                }
            }
            if(Right){
                model.player.rightFacing = true;
                model.player.x += step;
                model.player.animate();
            }
            if(Left){
                model.player.rightFacing = false;
                model.player.x -= step;
                model.player.animate();
            }
            if(Jump){
                if(model.player.jumpTimer < 8){
                    model.player.isOnPipe = false;
                    model.player.vertVelocity -= 8.8;
                }
            }
            if(Fire){
                model.addSprite(model.player.x, model.player.y, "Fireball");
                Fire = false;
            }
        }

        @Override
        public void run()
        {
            while(playing)
            {
                //long time = System.currentTimeMillis();
                this.update();
                model.update();
                view.update();

                try {
                    Thread.sleep(20);
                } catch(Exception e) {
                    Log.e("Error:", "sleeping");
                    System.exit(1);
                }
            }
        }

        void onTouchEvent(MotionEvent motionEvent)
        {
            int x = (int)motionEvent.getX();
            int y = (int)motionEvent.getY();
            switch (motionEvent.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN: // Player touched the screen
                    if(x < view.getWidth()/2 && y > view.getHeight()/2){
                        Left = true;
                    }else if(x > view.getWidth()/2 && y > view.getHeight()/2){
                        Right = true;
                    }else if(x < view.getWidth()/2 && y < view.getHeight()/2){
                        Fire = true;
                    }else{
                        Jump = true;
                    }
                    break;

                case MotionEvent.ACTION_UP: // Player withdrew finger
                    if(x < view.getWidth()/2 && y > view.getHeight()/2){
                        Left = false;
                    }else if(x > view.getWidth()/2 && y > view.getHeight()/2){
                        Right = false;
                    }else if(x < view.getWidth()/2 && y < view.getHeight()/2){
                        Fire = false;
                    }else{
                        Jump = false;
                    }
                    break;
            }
        }

        // Shut down the game thread.
        public void pause() {
            playing = false;
            try {
                gameThread.join();
            } catch (InterruptedException e) {
                Log.e("Error:", "joining thread");
                System.exit(1);
            }

        }

        // Restart the game thread.
        public void resume() {
            playing = true;
            gameThread = new Thread(this);
            gameThread.start();
        }
    }



    // Blayten K. Jones //
    //    10/27/2022    //
    //Creates a parent class for all visuals//
    static abstract class Sprite{
        int x, y, w, h;
        final int ground = 1050;
        int frames, imageIndex;
        boolean remove = false;
        GameView context;

        Sprite(int x, int y, GameView context){
            this.x = x; this.y = y;
            this.context = context;
            imageIndex = 0;
            lazyLoadImage();
        }


        abstract void update();
        abstract void draw(Canvas g, int scrollPos);
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
            return y <= colliderBottom;
        }

        void animate(){
            //Loops between the amount of frames an image has.
            imageIndex++;
            imageIndex %= frames;
        }
    }



    // Blayten K. Jones //
    //    9/28/2022     //
    //Contains the position of each individual pipe//

    static public class Pipe extends Sprite {
        static Bitmap pipeImage = null;

        Pipe(int x, int y, GameView context){
            super(x, y, context);
            frames = 1;
            h = pipeImage.getHeight(); w = pipeImage.getWidth();
        }


        void lazyLoadImage() {
            //Lazy loads the pipe image; if it is not found, it throws an error.
            try{
                if(pipeImage == null){
                    pipeImage = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.pipe);
                }
            }catch(Exception e){
                e.printStackTrace(System.err);
                System.exit(1);
            }
        }

        void draw(Canvas g, int scrollPos){
            g.drawBitmap(pipeImage, x - scrollPos, y, new Paint());
        }

        boolean existsHere(int x, int y){
            //Determines if a pipe exists at the position at these coordinates.
            return (x > this.x && x < (this.x + w)) && (y > this.y && y < (this.y + h));
        }


        @NonNull
        @Override
        public String toString(){
            return "Pipe (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h;
        }

        void update(){
        }
    }



    // Blayten K. Jones //
    //    10/27/2022    //
    //Enemy sprite that moves back and forth//

    static class Goomba extends Sprite{
        int px, py;
        double vertVelocity;
        static Bitmap[] animation = null;
        boolean flaming = false;
        boolean rightFacing = false;
        boolean isOnPipe = false;
        int deleteTimer = 0;

        Goomba(int x, int y, GameView context){
            super(x, y, context);
            h = animation[0].getHeight(); w = animation[0].getWidth();
        }

        void draw(Canvas g, int scrollPos){
            //Creates goomba's sprite.
            //Uses a ternary operator to determine if the goomba is facing left. If they are, it will add on the width to the position
            //and make its width negative to effectively flip the image around. Also determines if the goomba is flaming and changes sprite
            //accordingly.
            g.drawBitmap(animation[flaming ? 1 : 0], x - scrollPos, y, new Paint());
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
            x += (flaming ? 0 : (rightFacing ? 1 : -1)) * 5;

            //Increments the delete counter if the goomba is currently on fire.
            deleteTimer += (flaming ? 1 : 0);

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
                animation = new Bitmap[2];
                frames = 1;
                try{
                    animation[0] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.goomba);
                    animation[1] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.goomba_fire);
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
            return (x > this.x && x < (this.x + w)) && (y > this.y && y < (this.y + h));
        }

        @NonNull
        @Override
        public String toString(){
            return "Goomba (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h + "; Frame = " + imageIndex + "; Flaming = " + flaming;
        }
    }



    // Blayten K. Jones //
    //    10/27/2022    //
    //Projectile sprite that is released by Mario and kills enemies//

    static class Fireball extends Sprite {
        static Bitmap fireballImage;
        boolean rightFacing = true;
        int existenceTimer = 0;
        int groundX = 0;
        boolean hitGround = false;

        Fireball(int x, int y, GameView context, boolean right){
            super(x, y, context);
            w = fireballImage.getWidth(); h = fireballImage.getHeight();
            rightFacing = right;
            frames = 1;
        }

        void draw(Canvas g, int scrollPos){
            g.drawBitmap(fireballImage, x - scrollPos, y, new Paint());
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
                y = ground - 50 + 2*(int)(145.0 * -Math.abs((Math.sin(.025*(x - groundX)))));
            }
            //Moves the fireball left or right at a speed of 11.
            x += (rightFacing ? 1 : -1) * 11;
            //Removes the fireball after 116 cycles of existence (this allows the fireball to keep existing if the player is following it).
            remove = existenceTimer > 116;
        }

        void lazyLoadImage(){
            try{
                if(fireballImage == null){
                    fireballImage = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.fireball);
                }
            }catch(Exception e){
                e.printStackTrace(System.err);
                System.exit(1);
            }
        }

        @NonNull
        @Override
        public String toString(){
            return "Fireball (x,y) = (" + x + ", " + y + "), width = " + w + ", height = " + h + "; Frame = " + imageIndex;
        }
    }



    // Blayten K. Jones //
    //    10/13/2022    //
    //Contains the player character's sprites and position//

    static class Mario extends Sprite{
        int px, py;
        double vertVelocity;
        boolean rightFacing = true;
        static Bitmap[] animation = null;
        int jumpTimer;
        boolean isOnPipe = false;

        public Mario(int x, int y, GameView context){
            super(x, y, context);
            jumpTimer = 0;
            h = animation[0].getHeight(); w = animation[0].getWidth();
        }

        void lazyLoadImage() {
            //Lazy loads the animation for the player; if one of the images is not found, it throws an error.
            if(animation == null){
                animation = new Bitmap[5];
                frames = animation.length;
                try{
                    animation[0] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.mario1);
                    animation[1] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.mario2);
                    animation[2] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.mario3);
                    animation[3] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.mario4);
                    animation[4] = BitmapFactory.decodeResource(this.context.getResources(), R.drawable.mario5);
                }catch(Exception e){
                    System.out.println("A player image does not exist.");
                    System.exit(1);
                }
            }
        }

        void draw(Canvas g, int scrollPos){
            //Creates player's sprite.
            //Uses a ternary operator to determine if the player is facing left. If they are, it will add on the width to the position
            //and make its width negative to effectively flip the image around.
            g.drawBitmap(animation[imageIndex], x - scrollPos, y, new Paint());
        }

        void update(){
            vertVelocity += 2.4;
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
                jumpTimer = 8;
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
}
