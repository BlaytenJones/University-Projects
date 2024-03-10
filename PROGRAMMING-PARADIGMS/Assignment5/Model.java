// Blayten K. Jones //
//    9/15/2022     //
//Holds graphical representation//

import java.util.ArrayList;

class Model{
    ArrayList<Sprite> sprites;
    Mario player;

    Model(){
        sprites = new ArrayList<Sprite>();
        player = new Mario(100, 100);
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
                sprites.add(new Goomba(x, y));
                break;
            case "Fireball":
                sprites.add(new Fireball(x, y, player.rightFacing));
                break;
            case "Pipe":
                sprites.add(new Pipe(x, y));
                break;
        }
    }

    void unmarshal(Json ob){
        sprites = new ArrayList<Sprite>();
        sprites.add(player);
        Json tmpListPipe = ob.get("pipes");
        for(int i = 0; i < tmpListPipe.size(); i++){
            sprites.add(new Pipe(tmpListPipe.get(i)));
        }
        Json tmpListGoomba = ob.get("goombas");
        for(int i = 0; i < tmpListGoomba.size(); i++){
            sprites.add(new Goomba(tmpListGoomba.get(i)));
        }
    }

    Json marshal(){
        Json ob = Json.newObject();
        Json tmpListPipe = Json.newList();
        Json tmpListGoomba = Json.newList();
        ob.add("pipes", tmpListPipe);
        ob.add("goombas", tmpListGoomba);
        for(int i = 0; i < sprites.size(); i++){
            if(sprites.get(i).returnID() == "Pipe"){
                tmpListPipe.add(((Pipe)sprites.get(i)).marshal());
            }else if(sprites.get(i).returnID() == "Goomba"){
                tmpListGoomba.add(((Goomba)sprites.get(i)).marshal());
            }
        }
        return ob;
    }
}