(* source file advunit1.txt *)
(***************************************************************************)
(*      Introduction                                                       *)
(***************************************************************************)
PROCEDURE introduction(GS:PGameState);
BEGIN
   GS^.Content:=Wipe;
   GS^.Content+="<b>Welcome to mini-adventure!</b><hr noshade>"+
      "Your goal will be to find a treasure and bring it back to the starting point. "+
      "You will also get points for finding each location in the adventure. Points "+
      "will be deducted for various undesirable happenings: waking the ogre, getting "+
      "eaten, getting toasted, etc.<br><br>"+
      "I will guide you and be your eyes and ears. I will describe your location and "+
      "give you special instructions from time to time.<br><br>"+
      "To command me, simply click on the navigation buttons below.<hr noshade>"+
      "Click any button when you are ready to start your quest.";
   GS^.Content+='</div>'+LineEnding+'<div id="gamecontrols">'+
      '<div id="north" onclick="north()">North</div>'+LineEnding+
      '<div id="south" onclick="south()">South</div>'+LineEnding+
      '<div id="east"  onclick="east()" >East</div>'+LineEnding+
      '<div id="west"  onclick="west()" >West</div>'+LineEnding+
      '<div id="up"    onclick="up()"   >Up</div>'+LineEnding+
      '<div id="down"  onclick="down()" >Down</div>'+LineEnding+
      '<div id="quit"  onclick="quit()" >Quit</div></div>'+LineEnding+
      '<script language="JavaScript" type="text/javascript" src="Audio-HTML5.js"></script>'+LineEnding+
      '</body></html>';
END;

(*****************************************************************************)
(*      score                                                                *)
(*****************************************************************************)
FUNCTION score(GS:PGameState): INTEGER;
VAR
   loc:         rooms;
   sc:          INTEGER;

BEGIN
   sc           := 0;
   FOR loc := start TO flames DO
      IF GS^.visited[loc] THEN sc := sc + 10;
   IF NOT GS^.quit THEN sc := sc + 30;
   IF GS^.cooked THEN sc := sc - 50;
   IF GS^.eaten THEN sc := sc - 50;
   IF GS^.awake THEN sc := sc - 25;
   Result := sc;
   //GS^.Score:=Result;
END;

(*****************************************************************************)
(*      congratulations                                                      *)
(*****************************************************************************)
PROCEDURE congratulations(GS:PGameState);
BEGIN
   GS^.Msg:='';
   IF NOT GS^.cooked THEN BEGIN
        IF NOT GS^.quit THEN BEGIN
           GS^.Msg:="!!! Congratulations !!!<br><br>"+
              "You got out the treasure in only "+IntToStr(GS^.turns)+"turns.<hr noshade>";
        END;
        GS^.Msg+="You scored "+IntToStr(score(GS))+" points out of a maximum of 200 points.<br>"+
           "<br>So long for now, come again soon!";
   END
   ELSE GS^.Msg:="Sorry about that -- try again soon!";
END;

(*****************************************************************************)
(*      function whichway                                                    *)
(*****************************************************************************)
(* handled by the browser now                                                *)
(*****************************************************************************)

(*****************************************************************************)
(*      procedure noway                                                      *)
(*****************************************************************************)
PROCEDURE noway(GS:PGameState);
BEGIN
   GS^.Msg+="<BR>You cannot go in that direction.<br><br>";
END;
