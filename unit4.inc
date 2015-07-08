{%MainUnit: miniadv.p}
(* source file unit4.inc *)

(*****************************************************************************)
(*      procedure pnarrow2                                                   *)
(*****************************************************************************)
PROCEDURE pnarrow2(GS:PGameState);
BEGIN
   GS^.Msg+="You are in a very narrow passage. "+
      "To the west the passage opens out by a lake shore. To the east it is even "+
      "tighter. You might be able to squeeze through if you try real hard. "+
      "There is also a strange looking alcove in the south wall that seems to "+
      "open into a very dark tunnel.";
END;

PROCEDURE pnarrow2move(dir:directions;GS:PGameState);
begin
   CASE dir OF
      w:GS^.location := lakeshore;
      e:GS^.location := steam;
      s:GS^.location := ogreroom;
      n,u,d:noway(GS);
   END;
END;

(*****************************************************************************)
(*      procedure ppit                                                       *)
(*****************************************************************************)
PROCEDURE ppit(GS:PGameState);
BEGIN
   GS^.Msg+="You are at the bottom of a fifty foot pit. The walls are just a hair too steep "+
      "to climb. The pit is empty except for a few old dried bones - I can't tell if "+
      "they are human or not!!<br><br>In the center of the pit is a narrow shinny leading downward.";
END;

PROCEDURE ppitmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      d:GS^.location := ladder;
      u:BEGIN
          GS^.Msg+="<br>If you try to climb that, you're sure to kill yourself!<br><br>";
        END;
      n,s,e,w:noway(GS);
   END; {case}
END; {proc ppit}

(*****************************************************************************)
(*      procedure pcrystal                                                       *)
(*****************************************************************************)
PROCEDURE pcrystal(GS:PGameState);
BEGIN
   writeln ("You are in a shining hall of crystal. It is intensely cold but also chillingly");
   writeln ("beautiful. There are glass formations rising from the floor as if they had");
   writeln ("grown there, yet delicately sculptured with multi-faceted sides. An intense");
   writeln ("white light shines brilliantly from the floor, which is also made of a mirror");
   writeln ("smooth lead crystal. The light is almost blinding and the many reflections");
   writeln ("that it sets off among the crystal formations of the room make it almost");
   writeln ("impossible to tell where the room begins and where it ends.");
END;

PROCEDURE pcrystalmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      e:GS^.location := maze;
      n,w:GS^.location := ogreroom;
      s,u,d:noway(GS);
   END; {case}
END; {proc pcrystal}

(*****************************************************************************)
(*      procedure pbatscave                                                       *)
(*****************************************************************************)
PROCEDURE pbatscave(GS:PGameState);
BEGIN
   writeln ("You are in a steep cavern filled with shrieking vampire bats. They swoop and");
   writeln ("dive at you by the thousands. If I were you, I would get out as quickly as I");
   writeln ("could. There are openings to the west and to the north.");
END;

PROCEDURE pbatscavemove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      w:GS^.location := ogreroom;
      n:GS^.location := steam;
      e,s,u,d:noway(GS);
   END; {case}
END; {proc pbatscave}

(*****************************************************************************)
(*      procedure psteam                                                       *)
(*****************************************************************************)
PROCEDURE psteam(GS:PGameState);
BEGIN
   writeln ("You have entered a hall filled with a stifling steamy vapor. There are");
   writeln ("innumerable small geysers scattered about, each contributing its own steam");
   writeln ("to the general mist.");
   writeln ("To the west is a dark opening, as well as to the north. Further out in the");
   writeln ("middle of the room is a dark opening in the floor into which some of the");
   writeln ("vapor seems to be seeping.");
END;

PROCEDURE psteammove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      w:GS^.location := narrow2;
      n:GS^.location := deadend;
      d:GS^.location := maze;
      s:GS^.location := batscave;
      e,u:noway(GS);
   END; {case}
END; {proc psteam}

(*****************************************************************************)
(*      procedure pladder                                                       *)
(*****************************************************************************)
PROCEDURE pladder(GS:PGameState);
BEGIN
   writeln ("You are at the base of a huge ladder reaching up out of sight. It must extend");
   writeln ("up at least 500 feet, and it will take someone brave in heart to scale it.");
   writeln ("There are also passages which lead north and down.");
END;

PROCEDURE pladdermove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.location := maze;
      d:GS^.location := flames;
      u:IF GS^.carrying THEN BEGIN
           writeln ("You can't carry the treasure up the ladder --  it's much too heavy!");
        END
        ELSE GS^.location := vestibule;
        {endif}
      e,s,w:noway(GS);
   END; {case}
END; {proc pladder}

(*****************************************************************************)
(*      procedure pflames                                                       *)
(*****************************************************************************)
PROCEDURE pflames(GS:PGameState);
BEGIN
   writeln ("Unfortunately you have fallen into an underground fire pit. It is the source");
   writeln ("of heat that produces the geysers in the steam room. You have been toasted");
   writeln ("to a crisp, to put it politely.");
   GS^.cooked := true;
   GS^.done := true;
END; {proc pflames}

(*****************************************************************************)
(*      procedure pdeadend                                                       *)
(*****************************************************************************)
PROCEDURE pdeadend(GS:PGameState);
BEGIN
   writeln ("Dead end.");
END;

PROCEDURE pdeadendmove(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:GS^.location := steam;
      n,e,w,u,d:noway(GS);
   END; {case}
END; {proc pdeadend}
