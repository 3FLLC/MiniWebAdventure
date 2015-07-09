{%MainUnit: miniwebadv.p} (* (c) 2015 by 3F, LLC. see LICENSE.TXT *)
CONST
   nw   =  9;
   ne   =  5;
   sew  = 14;
   nonly=  1;
   nsew = 15;
   newud= 61;
   sw   = 10;
   ns   =  3;
   donly= 32;
   dn   = 33;
   ud   = 48;
   su   = 18;

type
   rooms=(start, grandroom, vestibule, narrow1, lakeshore, island, brink, iceroom,
          ogreroom, narrow2, pit, crystal, batscave, steam, deadend, ladder, maze, flames);

   directions=(n, s, e, w, u, d, q);
   mazerooms=(m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15,m16,m17,m18,m19);

   PRequest=^Request;
   Request=Record
      Method:String; // GET, POST, HEAD, etc.
      URI:String;
      Protocol:String;
      UserAgent:String;
      Status:Word;
      SendFile:Boolean;
      FileSize:LongWord;
      ContentLength:LongWord;
      PostData:String;
   End;

   GameStates=(unknown, badmethod, notfound, notallowed, filerequest, newgame);

   PGameState=^GameState;
   GameState=Record
      command:STRING[10];
      ch:CHAR;
      location:rooms;
      mazeloc:mazerooms;
      ogreloc:rooms;
      visited:ARRAY[start..flames] OF BOOLEAN;
      next:directions;
      twopow:ARRAY[n..d] OF INTEGER;
      turns:INTEGER;
      done:BOOLEAN;
      quit:BOOLEAN;
      eaten:BOOLEAN;
      awake:BOOLEAN;
      readmsg:BOOLEAN;
      carrying:BOOLEAN;
      dropped:BOOLEAN;
      trapped:BOOLEAN;
      cooked:BOOLEAN;
      points:longint;
      content:String;
      msg:String;
   End;

const
   dchars='qnsewud';

var
   GameStructure:PGameState;
   HTTPState:GameStates;
   rawHTTPHeader:TStringList;
   HTTPRequest:Request;

function roomsToString(loc:rooms):String;
begin
{$IFDEF TRACECODE}Writeln('@roomsToString');{$ENDIF}
   case loc of
      start:Result:='start';
      grandroom:Result:='grandroom';
      vestibule:Result:='vestibule';
      narrow1:Result:='narrow1';
      lakeshore:Result:='lakeshore';
      island:Result:='island';
      brink:Result:='brink';
      iceroom:Result:='iceroom';
      ogreroom:Result:='ogreroom';
      narrow2:Result:='narrow2';
      pit:Result:='pit';
      crystal:Result:='crystal';
      batscave:Result:='batscave';
      steam:Result:='steam';
      deadend:Result:='deadend';
      ladder:Result:='ladder';
      maze:Result:='maze';
      flames:Result:='flames';
   end;
end;

function StringToRooms(loc:String):rooms;
begin
{$IFDEF TRACECODE}Writeln('@StringToRooms');{$ENDIF}
   If loc='start' then Result:=start
   else If loc='vestibule' then Result:=vestibule
   else If loc='grandroom' then Result:=grandroom
   else If loc='narrow1' then Result:=narrow1
   else If loc='lakeshore' then Result:=lakeshore
   else If loc='island' then Result:=island
   else If loc='brink' then Result:=brink
   else If loc='iceroom' then Result:=iceroom
   else If loc='ogreroom' then Result:=ogreroom
   else If loc='narrow2' then Result:=narrow2
   else If loc='pit' then Result:=pit
   else If loc='crystal' then Result:=crystal
   else If loc='batscave' then Result:=batscave
   else If loc='steam' then Result:=steam
   else If loc='deadend' then Result:=deadend
   else If loc='ladder' then Result:=ladder
   else If loc='maze' then Result:=maze
   else If loc='flames' then Result:=flames;
end;

function MazeroomsToString(mazeloc:mazerooms):String;
begin
{$IFDEF TRACECODE}Writeln('@MazeroomsToString');{$ENDIF}
   case mazeloc of
      m1:Result:='m1';
      m2:Result:='m2';
      m3:Result:='m3';
      m4:Result:='m4';
      m5:Result:='m5';
      m6:Result:='m6';
      m7:Result:='m7';
      m8:Result:='m8';
      m9:Result:='m9';
      m10:Result:='m10';
      m11:Result:='m11';
      m12:Result:='m12';
      m13:Result:='m13';
      m14:Result:='m14';
      m15:Result:='m15';
      m16:Result:='m16';
      m17:Result:='m17';
      m18:Result:='m18';
      m19:Result:='m19';
   end;
end;

function StringToMazeRooms(mazeloc:String):mazerooms;
begin
{$IFDEF TRACECODE}Writeln('@StringToMazeRooms');{$ENDIF}
   case mazeloc of
      'm1':Result:=m1;
      'm2':Result:=m2;
      'm3':Result:=m3;
      'm4':Result:=m4;
      'm5':Result:=m5;
      'm6':Result:=m6;
      'm7':Result:=m7;
      'm8':Result:=m8;
      'm9':Result:=m9;
      'm10':Result:=m10;
      'm11':Result:=m11;
      'm12':Result:=m12;
      'm13':Result:=m13;
      'm14':Result:=m14;
      'm15':Result:=m15;
      'm16':Result:=m16;
      'm17':Result:=m17;
      'm18':Result:=m18;
      'm19':Result:=m19;
   end;
end;
