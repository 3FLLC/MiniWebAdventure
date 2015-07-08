{%MainUnit: miniwebadv.p} (* (c) 2015 by 3F, LLC. see LICENSE.TXT *)

/////////////////////////////////////////////////////////////////////////////
// Ability to read from the socket the HTTP request and populate a structure
// of what we need for the game. CodeRunner allows you to interact with the
// connection below a protocl - so this file implements all of the protocol
// pieces associated with the HTTP protocol (just what is needed).
// --------------------------------------------------------------------------
// Design benefit, since Modern Pascal's Code Runner handles the threading
// for you, all of your coded is written as if it was single-user. This means
// you get to benefit by using global variables which are only global to the
// current connection not other connections.
/////////////////////////////////////////////////////////////////////////////

function EscapeDecode(S:string):string;
var
   ch: Char;
   val: string;
   I:Integer;
   lResult:String;

begin
   lResult:=S;
   Result:='';
   I:=Pos('%',lResult);
   while I>0 do begin
      Result+=Copy(lResult,1,I-1);
      Val:=Copy(lResult,I+1,2);
      try
         Ch:=Chr(StrToInt('$'+Val));
         If ch=#0 and (Val='00') then Result+=Ch
         else if Ch<>#0 then Result+=Ch
         else Result+=Copy(lResult,I,3);
      except
         Result+=Copy(lResult,I,3);
      end;
      Delete(lResult,1,I+2);
      I:=Pos('%',lResult);
   end;
   Result:=Result+lResult;
end;

function EscapeEncode(S:string):string;
var
   Loop,MaxLoop:Integer;
   hPad:String;

begin
   Result:='';
   If S='' then Exit;
   MaxLoop:=Length(S);
   for Loop:=1 to MaxLoop do
      if S[Loop] in ['0'..'9','.','-'] then Result+=S[Loop]
      else if S[Loop]=#32 then Result+='%20'
      else if (S[Loop]='&') or (S[Loop]='%') or (S[Loop]=':') or (S[Loop]<#21) or (S[Loop]>#127) then begin
         hPad:=IntToHex(Ord(S[Loop]));
         if (hPad='91') or (hPad='92') then hPad:='27';
         Result+='%'+hPad;
      end
      else Result+=S[Loop]
end;

procedure CollectHeader;                       // Read network for HTTP Browser Request (HEADER ONLY)
var
   Ws:String;

begin
{$IFDEF TRACECODE}Writeln('@CollectHeader');{$ENDIF}
   rawHTTPHeader.Init;                         // Create INSTANCE of string list
   While Session.IsConnected do begin          // While browser is connected, loop
      If Session.IsReadable then begin         // If we received something from the browser/network
         If Session.CountWaiting=0 then Break; // If SYN_FIN packet (0 bytes) we are disconnected
         Ws:=Session.Readln(100);              // Upto 100ms or CR/LF read a string from the network
         If Ws<>'' then rawHTTPHeader.add(Ws)  // HTTP sends HEADER <blank line> BODY - is there data?
         Else Break;                           // Nope, then exit - end of HEADER.
      end
      else Yield(1);                           // CPU friendly, give up 1ms to other applications
{$IFDEF EMULATEBROWSER}
      If KeyPressed then
         If ReadKey=#27 then begin
            rawHTTPHeader.Add('GET / HTTP/1.1');
            rawHTTPHeader.Add('Host: ozznixon.com');
            rawHTTPHeader.Add('User-agent: Mozilla/5.0 (Linux; Android 4.0.4; Galaxy Nexus Build/IMM76B) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.133 Mobile Safari/535.19'); // Read agent for a DROID phone.
            rawHTTPHeader.Add('Accept: text/plain');
            rawHTTPHeader.Add('Accept-Charset: utf-8');
            rawHTTPHeader.Add('Accept-Encoding: gzip, deflate');
            rawHTTPHeader.Add('Accept-Language: en-US');
            rawHTTPHeader.Add('Connection: keep-alive');
            rawHTTPHeader.Add('Content-Type: application/x-www-form-urlencoded');
            rawHTTPHeader.Add('X-Requested-With: XMLHttpRequest');
            Writeln('* Set Faux Header');
            Break;
         End;
{$ENDIF}
   end;
end;

function ProcessRequest:GameStates;
var
   Ws:String;

begin
{$IFDEF TRACECODE}Writeln('@ProcessRequest');{$ENDIF}
   CollectHeader;
   Ws:=rawHTTPHeader.getStrings(0);           // First line of HTTP Header is GET / HTTP/1.0 (for example)
   rawHTTPHeader.setText(StringReplace(rawHTTPHeader.getText(),': ','=',[rfReplaceAll]));
   with HTTPRequest do begin
      ContentLength:=StrToIntDef(rawHTTPHeader.getValues("Content-Length"),0);
      PostData:='';
      SendFile:=False;
      Method:=Fetch(Ws,#32);
      If pos(Method,'GETPOST')=0 then begin
         Status:=405;
         result:=badmethod;
         Exit;
      end;
      URI:=Fetch(Ws,#32);
      Protocol:=Ws;
{$IFDEF CODERUNNER}Writeln('* Asked for ',URI,#13);{$ENDIF}
      if URI="/" then result:=newgame
      else begin
         URI:=EscapeDecode(URI);
         if copy(URI,1,1)<>'/' then begin
            Status:=403;
            result:=notallowed;
         end
         else if FileExists("."+URI) then begin // check permissions!
            SendFile:=True;
            Status:=200;
            result:=filerequest;
         end
         else Begin
            Status:=404;
            result:=unknown; // may end up being notfound!
         end;
      end;
   end;
end;

procedure LoadPostData;
var
   Ws:String;
   Timeout:Word;

begin
   Timeout:=0;
   If HTTPRequest.ContentLength=0 then Exit;
   While Session.IsConnected do begin
      If Session.IsReadable then begin
         If Session.CountWaiting=0 then Break;
         HTTPRequest.PostData+=Session.ReadStr(Session.CountWaiting,100);
         If Length(HTTPRequest.PostData)>=HTTPRequest.ContentLength then Exit;
      end
      else begin
         Yield(1);
         Inc(Timeout);
         If Timeout>12000 then exit; // too damn slow, abort!
      end;
   end;
end;

function ResponseCode(Code:Word):String;
begin
   Case Code of
      100:Result:='Continue';
      101:Result:='Switching Protocol';
      102:Result:='Processing';
      200:Result:='OK';
      201:Result:='Created';
      202:Result:='Accepted';
      203:Result:='Non-Authoritative Information';
      301:Result:='Moved Permanently';
      400:Result:='Bad Request';
      401:Result:='Unauthorized';
      402:Result:='Payment Required';
      403:Result:='Forbidden';
      404:Result:='Not Found';
      405:Result:='Method Not Allowed';
      408:Result:='Request Timeout';
      500:Result:='Internal Server Error';
      501:Result:='Not Implemented';
   End;
   Result:=IntToStr(Code)+#32+Result;
End;

procedure SendResponse(ConnBool:Boolean;MimeStr:String;GS:PGameState);
Const
   Connection=['close','keep-alive'];

Begin
   Session.Write(HTTPRequest.Protocol+#32+
      ResponseCode(HTTPRequest.Status)+LineEnding+
      'Access-Control-Allow-Origin: *'+LineEnding+
      'Allow: GET, HEAD'+LineEnding+
      'Connection: '+Connection[ConnBool]+LineEnding+
      'Content-Type: '+MimeStr+LineEnding+
      'Expires: Thu, 01 Dec 1994 16:00:00 GMT'+LineEnding+
      'Cache-Control: no-store'+LineEnding+
      'Pragma: no-cache'+LineEnding+
//      'WWW-Authentication: Basic'+LineEnding+
      'Server: ModernPascal2 (CodeRunner)'+LineEnding+
      'X-Powered-by: MiniWebAdventure.p'+LineEnding);
End;

function MimeEncoding(FileExtension:String):string;
Begin
   If FileExtension='' then begin
      Result:='text/plain';
      Exit;
   End;
   If FileExtension[1]='.' then Delete(FileExtension,1,1);
   if FileExtension='html' then Result:='text/html'
   else if FileExtension='htm' then Result:='text/html'
   else if FileExtension='css' then Result:='text/css'
   else if FileExtension='png' then Result:='image/png'
   else if FileExtension='json' then Result:='application/json'
   else Result:='text/plain';
end;

procedure UpdateBrowser(GS:PGameState);
var
   JSON:String;
   loc:rooms;

begin
{$IFDEF TRACECODE}Writeln('@UpdateBrowser');{$ENDIF}
   If GS^.Content<>'' then begin
      SendResponse(false,MimeEncoding('html'),GS);
      Session.Write('Content-Length: '+IntToStr(Length(GS^.Content))+LineEnding+
         LineEnding+GS^.Content);
   end
   else begin
      JSON:='{'+LineEnding+'"location":"';
      with GS^ do begin
         JSON+=roomsToString(location);
         JSON+='",'+LineEnding+'"done":';
         If done then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"quit":';
         If quit then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"cooked":';
         If cooked then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"eaten":';
         If eaten then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"awake":';
         If awake then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"readmsg":';
         If readmsg then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"carrying":';
         If carrying then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"trapped":';
         If trapped then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"dropped":';
         If dropped then JSON+='true,'+LineEnding
         else JSON+='false,'+LineEnding;
         JSON+='"turns":'+IntToStr(turns)+','+LineEnding+
            '"twopown":'+IntToStr(twopow[n])+','+LineEnding+
            '"twopows":'+IntToStr(twopow[s])+','+LineEnding+
            '"twopowe":'+IntToStr(twopow[e])+','+LineEnding+
            '"twopoww":'+IntToStr(twopow[w])+','+LineEnding+
            '"twopowu":'+IntToStr(twopow[u])+','+LineEnding+
            '"twopowd":'+IntToStr(twopow[d])+','+LineEnding;
         FOR loc:=start TO flames DO begin
            If Visited[loc] then JSON+='"visited'+roomsToString(loc)+'":true,'+LineEnding
            else JSON+='"visited'+roomsToString(loc)+'":false,'+LineEnding;
         // this generates an exception:   JSON+='"visited'+roomsToString(loc)+'":'+TF[GS^.visited[loc]];
         end;
         JSON+='"ts":'+IntToStr(TimeStamp)+','+LineEnding+
            '"msg":"'+Msg+'"'+LineEnding;
      End;
      JSON+='}';
      SendResponse(false,MimeEncoding('json'),GS);
      Session.Write('Content-Length: '+IntToStr(Length(JSON))+LineEnding+
         LineEnding+JSON);
   End;
end;

//{"location":"start","done":false,"quit":false,"cooked":false,"eaten":false,"awake":false,"readmsg":false,"carrying":false,"trapped":false,"dropped":false,"turns":1,"twopown":1,"twopows":2,"twopowe":4,"twopoww":8,"twopowu":16,"twopowd":32,"visitedstart":true,"visitedgrandroom":false,"visitedvestibule":false,"visitednarrow1":false,"visitedlakeshore":false,"visitedisland":false,"visitedbrink":false,"visitediceroom":false,"visitedogreroom":false,"visitednarrow2":false,"visitedpit":false,"visitedcrystal":false,"visitedbatscave":false,"visitedsteam":false,"visiteddeadend":false,"visitedladder":false,"visitedmaze":false,"visitedflames":false,"ts":1436289698,"msg":"You can't jump to the clouds!!<hr noshade>You are standing by a hole in the ground. It looks big enough to climb down."}
procedure BrowserUpdate(GS:PGameState);
var
   Ws:String;
   StrList:TStringList;
   loc:rooms;
   oldts:TTimeStamp;

begin
   HTTPRequest.Status:=200;
   if HTTPRequest.PostData[1]='{' then begin
      Ws:=Copy(HTTPRequest.PostData,2,HTTPRequest.ContentLength-2); // should drop braces
      Ws:=StringReplace(Ws,'"','',[rfReplaceAll]);
      Ws:=StringReplace(Ws,':','=',[rfReplaceAll]);
      Ws:=StringReplace(Ws,',',#13#10,[rfReplaceAll]);
      StrList.Init;
      StrList.setText(Ws);
      With GS^ do begin
         location:=StringToRooms(StrList.getValues("location"));
         done:=StrList.getValues("done")="true";
         quit:=StrList.getValues("quit")="true";
         cooked:=StrList.getValues("cooked")="true";
         eaten:=StrList.getValues("eaten")="true";
         awake:=StrList.getValues("awake")="true";
         readmsg:=StrList.getValues("readmsg")="true";
         carrying:=StrList.getValues("carrying")="true";
         trapped:=StrList.getValues("trapped")="true";
         dropped:=StrList.getValues("dropped")="true";
         turns:=StrToIntDef(StrList.getValues("turns"),1);
         twopow[n]:=StrToIntDef(StrList.getValues("topdown"),1);
         twopow[s]:=StrToIntDef(StrList.getValues("topdows"),2);
         twopow[e]:=StrToIntDef(StrList.getValues("topdowe"),4);
         twopow[w]:=StrToIntDef(StrList.getValues("topdoww"),8);
         twopow[u]:=StrToIntDef(StrList.getValues("topdowu"),16);
         twopow[d]:=StrToIntDef(StrList.getValues("topdowd"),32);
         FOR loc:=start TO flames DO
            visited[loc]:=StrList.getValues("visited"+roomsToString(loc))="true";
         oldts:=StrToIntDef(StrList.getValues("ts"),0);
         if (oldts<timestamp-120000) then begin // 2 minutes!
            HTTPRequest.Status:=408;
         end;
      End;
      StrList.Free;
   end
   else HTTPRequest.Status:=400;
end;
