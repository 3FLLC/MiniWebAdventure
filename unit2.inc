{%MainUnit: miniadv.p}
(* source file unit2.inc *)

(*****************************************************************************)
(*      procedure ogreaction                                                 *)
(*****************************************************************************)
PROCEDURE ogreaction(GS:PGameState);
BEGIN
   IF NOT GS^.awake THEN BEGIN
      GS^.Msg+="<br>This is the ogre's lair! If you are not careful, you will wake him.";
      IF (GS^.turns MOD 7 )=0 THEN BEGIN
         GS^.awake := true;
         GS^.Msg+="<br>Now you have done it! You woke the ogre - better get out while you still can!!";
      END;
   END
   ELSE BEGIN
      GS^.Msg+="<br>You would not listen to me would you? You really better get out "+
         "of here before you get eaten!";

      IF GS^.carrying THEN
        IF (GS^.turns MOD 2) = 0 THEN BEGIN
           GS^.Msg+="<br>Too bad!! The ogre caught you and roasted you for dinner."+
              "<br>Better luck next time!!";
           GS^.eaten := true;
           GS^.quit  := true;
        END
        ELSE BEGIN
           GS^.Msg+="<br>Get out fast if you want to avoid being a meal for the ogre!!";
        END
      ELSE
        IF (GS^.turns MOD 5) = 0 THEN BEGIN
           GS^.Msg+="<br>Too bad - you have been eaten!";
           GS^.eaten := true;
           GS^.quit := true;
        END
   END;
END;

(*****************************************************************************)
(*      procedure pstart                                                     *)
(*****************************************************************************)
PROCEDURE pstart(GS:PGameState);
BEGIN
   IF GS^.carrying THEN GS^.done := true
   ELSE BEGIN
      GS^.Msg+="You are standing by a hole in the ground. It looks big enough to climb down.";
   END;
END;

PROCEDURE pstartmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir of
      n,s,e,w:noway(GS);
      u:GS^.Msg+="You cannot jump to the clouds!!<hr noshade>";
      d:GS^.location:=vestibule;
   END;
END;

(*****************************************************************************)
(*      procedure pvestibule                                                 *)
(*****************************************************************************)
PROCEDURE pvestibule(GS:PGameState);
BEGIN
   GS^.Msg+="You are in the entrance to a cave of passageways. There are halls "+
      "leading off to the north, south,and east. Above you is a tunnel"+
      "leading to the surface.";
END;

(*** // requires more buttons on the browser -- implement if you want!
   IF GS^.dropped THEN BEGIN
      GS^.Msg+="<br>To the north, through a narrow crack,you can see the treasure. If you ";
         "stretch your arm through you might reach it. Do you want to try?");
      IF IsYes THEN BEGIN
        carrying := true;
        dropped := false;
      END;
   END;
***)

PROCEDURE pvestibulemove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.location := narrow1;
      s:GS^.location := grandroom;
      e:GS^.location := iceroom;
      w,d:noway(GS);
      u:GS^.location := start;
   END;
END;

(*****************************************************************************)
(*      procedure pgrandroom                                                 *)
(*****************************************************************************)
PROCEDURE pgrandroom(GS:PGameState);
BEGIN
   GS^.Msg+="You are in a huge open room, with an immense expanse of ceiling. "+
      "A dark passage leads west and a narrow crack leads downward.";
END;

PROCEDURE pgrandroommove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      w:GS^.location := brink;
      d:GS^.location := iceroom;
      n,s,e,u:noway(GS);
   END;
END;

(*****************************************************************************)
(*      procedure pnarrow1                                                   *)
(*****************************************************************************)
PROCEDURE pnarrow1(GS:PGameState);
BEGIN
   GS^.Msg+="You are in a narrow passage which continues to the north. It is "+
      "extremely narrow to the south.<br><br>"+
      "A very tight crawl also leads east..."+
      " A curious odor seeps through it. I would think twice before trying "+
      "to go that way!";
   IF GS^.carrying THEN BEGIN
      GS^.Msg+="The treasure will not fit through the crack going south.";
(***
         " Do you want to leave it here?");
      IF IsYes THEN BEGIN
        dropped := true;
        carrying := false;
      END;
***)
   END;
END;

PROCEDURE pnarrow1move(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.location := lakeshore;
      e:GS^.location := ogreroom;
      s:GS^.Msg+="It's too narrow to get through!<br>";
      w,u,d:noway(GS);
   END;
END;

(*****************************************************************************)
(*      procedure plakeshore                                               *)
(*****************************************************************************)
PROCEDURE plakeshore(GS:PGameState);
BEGIN
   GS^.Msg+="You are on the shore of a vast underground lake. Narrow passages "+
      "wind away to the east and south. A small island is visible in the "+
      "center of the lake to the north.";
END;

PROCEDURE plakeshoremove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.location := island;
      s:GS^.location := narrow1;
      e:GS^.location := narrow2;
      w,u,d:noway(GS);
   END;
END;

(*****************************************************************************)
(*      procedure pisland                                                    *)
(*****************************************************************************)
PROCEDURE pisland(GS:PGameState);
BEGIN
   GS^.Msg+="You are on a small island in the middle of a huge underground lake. "+
      "Dark frigid waters surround you on all sides. You can barely make out "+
      "the shoreline to the south. A small message is scratched in the "+
      "dirt here. It says: 'The treasure may be found in the maze.'";
   GS^.readmsg := true;
END;

PROCEDURE pislandmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n,e,w,u,d:noway(GS);
      s:GS^.location := lakeshore;
   END;
END;

(*****************************************************************************)
(*      procedure pbrink                                                     *)
(*****************************************************************************)
PROCEDURE pbrink(GS:PGameState);
BEGIN
   GS^.Msg+="You are on the bottom of a steep incline. The bottom of the pit "+
      "is over 50 feet below you. You could probably slide down safely "+
      "but I cannot promise you that you could get back up.<br><br>"+
      "To the west is a dark opening into a rubble-filled tunnel. A "+
      "vampire bat just flew out of it shreiking.";
END;

PROCEDURE pbrinkmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n,s,e,u:noway(GS);
      w:GS^.location := ogreroom;
      d:GS^.location := pit;
   END;
END;

(*****************************************************************************)
(*      procedure piceroom                                                   *)
(*****************************************************************************)
PROCEDURE piceroom(GS:PGameState);
BEGIN
   GS^.Msg+="You are in a room whose walls are made of a deep blue crystalline ice. To the "+
      "north a narrow tunnel opens. From the other end of the tunnel an ominous "+
      "growling sound may be heard. To the east a sparkling luminescence emanates "+
      "from a broad opening. To the west a passage leads back to the vestibule.";
END;

PROCEDURE piceroommove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      e:GS^.location := crystal;
      n:GS^.location := ogreroom;
      w:GS^.location := vestibule;
      s,u,d:noway(GS);
   END;
END;

(*****************************************************************************)
(*      procedure pogreroom                                                  *)
(*****************************************************************************)
PROCEDURE pogreroom(GS:PGameState);
BEGIN
   GS^.Msg+="You are in a low room whose walls are covered with ominous dark blots "+
      "of dried blood. The center of the room is dominated by a firepit, which "+
      "contains burned-out coals and a long spit suspended over it's center. "+
      "From one dark corner emanates a horrible growling noise like that "+
      "of some unspeakable monster snoring during it's dream of rending you limb "+
      "from limb and making you it's dinner!";
   ogreaction(GS);
   IF NOT GS^.eaten THEN
      GS^.Msg+=" There are exits to the east, west, north, and south.";
END;

PROCEDURE pogreroommove(dir:directions;GS:PGameState);
var
   i,j:INTEGER;

BEGIN
   CASE dir OF
      w:GS^.location := narrow1;
      e:GS^.location := batscave;
      n:GS^.location := narrow2;
      s:GS^.location := iceroom;
      d:BEGIN
         GS^.quit := true;
         GS^.eaten := true;
         GS^.Msg+="Oh No!! You dummy!!!!<br><br>"+
            "You just fell into the firepit and made such a ruckus that "+
            "you woke the ogre. I hate to tell you this... but you are also trapped!"
            "<hr noshade>You have been added to the ogre's gourmet recipe library!<br>"+
            "Better luck next time!";
      END;
      u:noway(GS);
   END;
END;
