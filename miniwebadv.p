program MiniWebAdventure; (* (c) 2015 by 3F, LLC. see LICENSE.TXT *)

/////////////////////////////////////////////////////////////////////////////
// Miniature Web Adventure Game
// =========================================================================
// Author: G.E. Ozz Nixon Jr.
// Inspiration: Ported adv1.pas from 1984 to Modern Pascal 2.0
//              Ported miniadv.p from 2015 to a web based CodeRunner App
/////////////////////////////////////////////////////////////////////////////

uses
{$IFNDEF CODERUNNER}
   display,     // incoporate keypressed/readkey when debugging
{$ENDIF}
   environment, // incorporate FileExist
   classes,     // incorporate support for classes and objects
   strings,     // incorporate stringlist and string manipulation routines
   sockets;     // incorporate the ability to interact with the network/browser

{$IFNDEF CODERUNNER}
{$DEFINE TRACECODE}
{$DEFINE EMULATEBROWSER}
/////////////////////////////////////////////////////////////////////////////
// Mimic Session to make this compilable in the debugger
/////////////////////////////////////////////////////////////////////////////
Type
   TSession=Class
      PlaceHolder:Boolean;
   End;

function TSession.IsConnected:Boolean; begin result:=true; end;
function TSession.IsReadable:Boolean; begin result:=false; end;
function TSession.CountWaiting:Longint; begin result:=0; end;
procedure TSession.Write(S:String); begin System.Write(S); end;
procedure TSession.Writeln(S:String); begin System.Writeln(S); end;
function TSession.Readln(ms:Longint):String; begin yield(ms); result:=''; end;

Var
   Session:TSession;
{$ENDIF}

/////////////////////////////////////////////////////////////////////////////
// Global Structure Definition
/////////////////////////////////////////////////////////////////////////////
{$include mwa_struct.inc}

/////////////////////////////////////////////////////////////////////////////
// Interact with a web browser - include file
/////////////////////////////////////////////////////////////////////////////
{$include mwa_http.inc}

FUNCTION wipe:string;
BEGIN
   Result:='<!DOCTYPE html><html><head>'+LineEnding+
      '<title>Mini-Web-Adventure v1.1</title>'+LineEnding+
      '<link rel="stylesheet" type="text/css" href="style.css">'+LineEnding+
      '<meta property="og:image" content="https://aahabershaw.files.wordpress.com/2012/09/aowheroes.jpg"/>'+LineEnding+
      '<script language="JavaScript" type="text/javascript" src="navig8r.js"></script>'+LineEnding+
      '<body>'+LineEnding+
      '<img src="MiniAdventureLogo.png">'+LineEnding+
      '<div id="gamecontent">'+LineEnding;
END;

/////////////////////////////////////////////////////////////////////////////
// Initialize()
/////////////////////////////////////////////////////////////////////////////
PROCEDURE Initialize(GS:PGameState);
VAR
   loc:rooms;

BEGIN
{$IFDEF TRACECODE}Writeln('@Initialize');{$ENDIF}
   with GS^ do begin
      location:=start;
      done:=false;
      quit:=false;
      cooked:=false;
      eaten:=false;
      awake:=false;
      readmsg:=false;
      carrying:=false;
      trapped:=false;
      dropped:=false;
      turns:=0;
      twopow[n]:=1;
      twopow[s]:=2;
      twopow[e]:=4;
      twopow[w]:=8;
      twopow[u]:=16;
      twopow[d]:=32;
      FOR loc:=start TO flames DO visited[loc]:=false;
      points:=0;
      content:='';
      msg:='';
   end;
END;

{$include_once unit1.inc}
{$include_once unit2.inc}
{$include_once unit3.inc}
{$include_once unit4.inc}


procedure VerifyMove(GS:PGameState;dir:directions);
var
   sourceroom:rooms;

begin
   if (dir!=q) then begin
      Inc(GS^.Turns);
      GS^.Msg:='';
      sourceroom:=GS^.location;
      CASE GS^.location OF
         start:          pstartmove(dir,GS);
         grandroom:      pgrandroommove(dir,GS);
         vestibule:      pvestibulemove(dir,GS);
         narrow1:        pnarrow1move(dir,GS);
         lakeshore:      plakeshoremove(dir,GS);
         island:         pislandmove(dir,GS);
         brink:          pbrinkmove(dir,GS);
         iceroom:        piceroommove(dir,GS);
         ogreroom:       pogreroommove(dir,GS);
         narrow2:        pnarrow2move(dir,GS);
         pit:            ppitmove(dir,GS);
         crystal:        pcrystalmove(dir,GS);
         batscave:       pbatscavemove(dir,GS);
         steam:          psteammove(dir,GS);
         deadend:        pdeadendmove(dir,GS);
         ladder:         pladdermove(dir,GS);
         maze:           pmazemove(dir,GS);
         //no such thing - you are dead: flames:         pflamesmove(dir,GS);
      END; {case}
      GS^.visited[GS^.location] := true;
      CASE GS^.location OF
         start:          pstart(GS);
         grandroom:      pgrandroom(GS);
         vestibule:      pvestibule(GS);
         narrow1:        pnarrow1(GS);
         lakeshore:      plakeshore(GS);
         island:         pisland(GS);
         brink:          pbrink(GS);
         iceroom:        piceroom(GS);
         ogreroom:       pogreroom(GS);
         narrow2:        pnarrow2(GS);
         pit:            ppit(GS);
         crystal:        pcrystal(GS);
         batscave:       pbatscave(GS);
         steam:          psteam(GS);
         deadend:        pdeadend(GS);
         ladder:         pladder(GS);
         maze:begin
            if sourceroom<>maze then GS^.mazeloc:=m1;
            pmaze(GS);
         end;
         flames:         pflames(GS);
      END; {case}
   end
   else GS^.Quit:=True;
   if GS^.quit OR GS^.done then congratulations(GS);
   UpdateBrowser(GS)
end;

Procedure Main;
var
   GS:PGameState;
   FH:Longint;
   Buf:String;
   BR:Longint;

begin
{$IFDEF TRACECODE}Writeln('@Main');{$ENDIF}
   New(GS);
   HTTPState:=ProcessRequest;
   case HTTPState of
      unknown:begin
         // here we check to see if it is a GAME move:
         if HTTPRequest.Method="POST" then begin
            if HTTPRequest.ContentLength=0 then begin
               HTTPRequest.Status:=200;
               Initialize(GS);
               GS^.visited[start] := true;
               pstart(GS);
               UpdateBrowser(GS);
            end
            else begin
               LoadPostData;
               BrowserUpdate(GS);
               if HTTPRequest.URI='/north' then verifymove(GS,n)
               else if HTTPRequest.URI='/south' then verifymove(GS,s)
               else if HTTPRequest.URI='/east' then verifymove(GS,e)
               else if HTTPRequest.URI='/west' then verifymove(GS,w)
               else if HTTPRequest.URI='/up' then verifymove(GS,u)
               else if HTTPRequest.URI='/down' then verifymove(GS,d)
               else if HTTPRequest.URI='/quit' then verifymove(GS,q)
               else
                  // otherwise send known error:
                  SendResponse(false,'text/plain',GS);
            end;
         end
         else begin
            HTTPRequest.Status:=405;
            // otherwise send known error:
            SendResponse(false,'text/plain',GS);
         end;
      end;
      notfound:begin
         SendResponse(false,'text/plain',GS);
      end;
      notallowed:begin
         SendResponse(false,'text/plain',GS);
      end;
      filerequest:begin
         if HTTPRequest.SendFile then begin
            FH:=FileOpen("."+HTTPRequest.URI, fmShareDenyNone);
            If FH>-1 then begin
               SendResponse(false,MimeEncoding(ExtractFileExtension(HTTPRequest.URI)),GS);
               Session.Writeln('Content-Length: '+IntToStr(FileSize(FH))+LineEnding);
               SetLength(Buf,4096);
               While not EndOfFile(FH) do begin
                  BR:=FileRead(FH,@Buf[1],4096);
                  Session.Write(Copy(Buf,1,BR));
               End;
               SetLength(Buf,0);
               FileClose(FH);
            end
            else begin
               HTTPRequest.Status:=403;
               SendResponse(false,'text/plain',GS);
            end;
         end
         else begin
            HTTPRequest.Status:=500;
            SendResponse(false,'text/plain',GS);
         end;
      end;
      newgame:begin
         Introduction(GS);
         UpdateBrowser(GS);
      end;
   end;
   Dispose(GS);
end;

Begin
{$IFDEF CODERUNNER}Writeln('* Connection from '+Session.GetPeerIPAddress,#13);{$ENDIF}
{$IFDEF TRACECODE}Writeln('@jump_in');{$ENDIF}
   Main;
{$IFDEF TRACECODE}Writeln('@jump_out');{$ENDIF}
end.
