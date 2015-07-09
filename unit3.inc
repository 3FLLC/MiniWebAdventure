{%MainUnit: miniadv.p}
(* source file unit3.inc *)

(*************************************************************************)
(*   procedure pmaze                                                                    *)
(*************************************************************************)
PROCEDURE describe(wh:INTEGER;GS:PGameState);

   FUNCTION bittest (v: INTEGER; dir: directions; GS:PGameState): BOOLEAN;
   BEGIN
      Result:=((v DIV GS^.twopow[dir]) MOD 2) = 1;
   END; (* function bittest *)

VAR
   Ws:String;

BEGIN
   GS^.Msg+="You are in a maze of featureless passages. There are exits visible "+
      "in the following directions: ";
   Ws:='';
   if bittest (wh,n,GS)  THEN Ws+=',north ';
   if bittest (wh,s,GS)  THEN Ws+=',south ';
   if bittest (wh,e,GS)  THEN Ws+=',east ';
   if bittest (wh,w,GS)  THEN Ws+=',west ';
   if bittest (wh,u,GS)  THEN Ws+=',up ';
   if bittest (wh,d,GS)  THEN Ws+=',down ';
   GS^.Msg+=Copy(Ws,2,Length(Ws)-1)+".";
END; (* procedure describe *)

PROCEDURE sameplace(GS:PGameState);
BEGIN
   GS^.Msg+="<br>You have crawled around some twisted tunnels and wound up where you began.<hr noshade>";
END; (* proc sameplace *)

PROCEDURE treasure(GS:PGameState);
BEGIN
   IF NOT GS^.carrying THEN BEGIN
      IF GS^.readmsg THEN BEGIN
         GS^.Msg+="<b>The treasure is here!!</b><br>You have picked it up and are ready to return to the start.";
         GS^.Carrying:=True;
            //writeln ("Do you want to take it now?");
            //Command:=Uppercase(ReadKey);
            //IF (command = 'Y') OR (command = 'YES') THEN carrying := true;
      END
      ELSE BEGIN
         GS^.Msg+="The light is extremely dim here. "+
            "You better get out or risk falling into a pit.";
      END; (* if readmsg *)
   END; (* if not carrying *)
END; (* proc treasure *)

PROCEDURE pm1(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:GS^.location := ladder;
      e:GS^.mazeloc  := m2;
      w:GS^.mazeloc  := m4;
      u:GS^.location := steam;
      n,d:noway(GS);
   END; (* case *)
END; (* proc pm1 *)


PROCEDURE pm2(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m1;
      w:sameplace(GS);
      e,s,u,d:noway(GS);
    END; (* case *)
END; (* proc pm2*)

PROCEDURE pm3(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m1;
      e:sameplace(GS);
      w,s,u,d:noway(GS);
    END; (* case *)
END; (* proc pm3*)

PROCEDURE pm4(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:GS^.mazeloc:= m7;
      e:GS^.mazeloc:= m3;
      w:GS^.mazeloc:= m5;
      n,u,d:noway(GS);
    END; (* case *)
END; (* proc pm4*)

PROCEDURE pm5(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m1;
      s,e,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm5*)

PROCEDURE pm6(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m4;
      e:sameplace(GS);
      s,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm6*)

PROCEDURE pm7(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m5;
      s:GS^.mazeloc:= m9;
      e:GS^.mazeloc:= m6;
      w:GS^.mazeloc:= m8;
      u,d:noway(GS);
    END; (* case *)
END; (* proc pm7*)


PROCEDURE pm8(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m5;
      w:sameplace(GS);
      e,s,u,d:noway(GS);
    END; (* case *)
END; (* proc pm8*)


PROCEDURE pm9(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:GS^.mazeloc:= m11;
      w:GS^.mazeloc:= m10;
      n,e,u,d:noway(GS);
    END; (* case *)
END; (* proc pm9*)


PROCEDURE pm10(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:sameplace(GS);
      n:GS^.mazeloc:= m8;
      e,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm10*)


PROCEDURE pm11(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m9;
      e:GS^.mazeloc:= m6;
      w:GS^.mazeloc:= m10;
      u:GS^.mazeloc:= m1;
      d:GS^.mazeloc:= m12;
      s:noway(GS);
    END; (* case *)
END; (* proc pm11*)


PROCEDURE pm12(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m13;
      d:GS^.mazeloc:= m16;
      e,s,w,u:noway(GS);
    END; (* case *)
END; (* proc pm12*)

PROCEDURE pm13(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m14;
      d:GS^.mazeloc:= m17;
      e,s,w,u:noway(GS);
    END; (* case *)
END; (* proc pm13*)

PROCEDURE pm14(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m15;
      d:GS^.mazeloc:= m18;
      e,s,w,u:noway(GS);
    END; (* case *)
END; (* proc pm14*)

PROCEDURE pm15(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      u:GS^.mazeloc:= m1;
      d:GS^.mazeloc:= m19;
      n,e,s,w:noway(GS);
    END; (* case *)
END; (* proc pm15*)

PROCEDURE pm16(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m17;
      s:sameplace(GS);
      e,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm16*)


PROCEDURE pm17(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m18;
      s:GS^.mazeloc:= m16;
      e,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm17*)

PROCEDURE pm18(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      n:GS^.mazeloc:= m19;
      s:GS^.mazeloc:= m17;
      e,w,u,d:noway(GS);
    END; (* case *)
END; (* proc pm18*)

PROCEDURE pm19(dir:directions;GS:PGameState);
BEGIN
   CASE dir OF
      s:GS^.mazeloc:= m18;
      u:GS^.mazeloc:= m15;
      n,e,w,d:noway(GS);
    END; (* case *)
END; (* proc pm19*)


PROCEDURE pmaze(GS:PGameState);
BEGIN (* proc pmaze *)
   //mazeloc := m1;
   CASE GS^.mazeloc OF
      m1:GS^.Msg+="You are in a maze of featureless passages. "+
         "From here you can go south, east, west or up.";
      m2:describe(nw,GS);
      m3:describe(ne,GS);
      m4:describe(sew,GS);
      m5:describe(nonly,GS);
      m6:describe(ne,GS);
      m7:describe(nsew,GS);
      m8:describe(nw,GS);
      m9:describe(sw,GS);
      m10:describe(ns,GS);
      m11:describe(newud,GS);
      m12:describe(dn,GS);
      m13:describe(dn,GS);
      m14:describe(dn,GS);
      m15:describe(ud,GS);
      m16:describe(ns,GS);
      m17:describe(ns,GS);
      m18:describe(ns,GS);
      m19:begin
         describe(su,GS);
         treasure(GS);
      end;
   END; (* case *)
END; (* proc pmaze *)

PROCEDURE pmazemove(dir:directions;GS:PGameState);
BEGIN (* proc pmaze *)
   CASE GS^.mazeloc OF
      m1:pm1(dir,GS);
      m2:pm2(dir,GS);
      m3:pm3(dir,GS);
      m4:pm4(dir,GS);
      m5:pm5(dir,GS);
      m6:pm6(dir,GS);
      m7:pm7(dir,GS);
      m8:pm8(dir,GS);
      m9:pm9(dir,GS);
      m10:pm10(dir,GS);
      m11:pm11(dir,GS);
      m12:pm12(dir,GS);
      m13:pm13(dir,GS);
      m14:pm14(dir,GS);
      m15:pm15(dir,GS);
      m16:pm16(dir,GS);
      m17:pm17(dir,GS);
      m18:pm18(dir,GS);
      m19:pm19(dir,GS);
   END; (* case *)
END; (* proc pmaze *)
