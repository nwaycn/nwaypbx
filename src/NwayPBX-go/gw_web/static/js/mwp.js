/***********************************************************************

Copyright(C) MYVOIPAPP, All Rights Reserved.

Author: Gilson Yi.

***********************************************************************/

/***********************************************************************
**
** Data
**
***********************************************************************/
var uaState = 0;
var nonce = "";
var localSDPInfo='';   // crypto information
var localCandidate=''; // address information
var localIceufrag='';  // local ice-ufrag
var localIcePwd='';    // local ice-pwd

var mssCallID=0;
var mssSDPInfo='';
var mssCandidate='';

var wsSocket=0;
var localAudioStream=0;
var peerConnection=0;

/***********************************************************************
**
** configurations
**
***********************************************************************/
mssConfig = {
    addr:   "192.168.1.103",
    port:   5070,
    user:   "100",
    password:"100"    
};

/***********************************************************************
**
** Cookie functions
**
***********************************************************************/
function ckSave()
{
    var ldb = window.localStorage;
    ldb.mssAddr = escape(mssConfig.addr);
    ldb.mssPort = escape(mssConfig.port);
    ldb.mssUser = escape(mssConfig.user);
    ldb.mssPassword = escape(mssConfig.password);
}

function ckRetrieve()
{
    var ldb = window.localStorage;
    var addr = ldb.mssAddr;
    if(addr==null)
    {
        return -1;
    }
    mssConfig.addr = ldb.mssAddr;
    mssConfig.port = ldb.mssPort;
    mssConfig.user = ldb.mssUser;
    mssConfig.password = ldb.mssPassword;
    return 0;
}

/***********************************************************************
**
** UI functions
**
***********************************************************************/
function InputToInt(input_name)
{
    var value = $("#"+input_name).val();
    if( null == value || !value.length )
    {
        return -1;
    }
    
    return new Number(value);
}

function showAccountDlg()
{
    $('#edtAddr').val(mssConfig.addr);
    $('#edtPort').val(mssConfig.port);
    $('#edtUsrName').val(mssConfig.user);
    $('#edtUsrPwd').val(mssConfig.password);
    
    $('#accountDlg').modal({backdrop:false, show:true});
}

function uiSubmitAcc()
{
    mssConfig.addr = $('#edtAddr').val();
    mssConfig.port = InputToInt("edtPort");
    mssConfig.user = $('#edtUsrName').val();
    mssConfig.password = $('#edtUsrPwd').val();
    
    ckSave();
    
    ccEnterState(UA_STATE.UNREG);
    
    initWebPhoneUI();
    wrtcStartAudioStream();
}


function uiDisableEditBox(box_name)
{
    $("#"+box_name).attr("disabled", "disabled");
}

function uiEnableEditBox(box_name)
{
    $("#"+box_name).removeAttr("disabled");
}

function uiEditBoxHasVal(box_name)
{
    var value = $("#"+box_name).val()    
    if( value == null || value == "" )
    {
        return false;
    }
    
    return true;
}

function uiConvertSpecialKey(key)
{
    switch(key)
    {
    case 10:
        return '*';
        
    case 11:
        return '#';
        
    default:
        return key;
    }
}

function uiAppendVal(digit)
{
    var val = $("#displayPad").val();
    val += digit;
    $("#displayPad").val(val);
}

function uiDisplay(text)
{
    $("#displayPad").val(text);
}

function uiClearVal()
{
    $("#displayPad").val('');
}


function initWebPhoneUI()
{
    uiDisableEditBox("displayPad");
    uiDisableEditBox("btnCall");
    uiDisableEditBox("btnDecline");
}

function uiCall()
{
    dbgTrace("uiCall");
    switch(uaState)
    {
    case UA_STATE.IDLE:
        uiPrepareOCall();
        ccOCall();
        break;
        
    case UA_STATE.T_ALERT:
        uiStopAlertTone();
        ccTAnswer();
        break;
        
    default:
        break;
    }
}

function uiDecline()
{
    dbgTrace("uiDecline");
        
    wrtcStopPC();
    
    ccEndCall();
    
    uiStopAlertTone();
    uiStopRingBackTone();
}

function uiKeyClicked(key)
{
    var digit = uiConvertSpecialKey(key);
    
    uiAppendVal(digit);    
    ccSendDTMF(digit);
}

function uiPrepareIdle()
{
    uiClearVal();
    
    uiEnableEditBox("displayPad");    
    uiEnableEditBox("btnCall");
    uiDisableEditBox("btnDecline");
}

function uiPrepareOCall()   // update UI for preparing outgoing call
{   
    uiEnableEditBox("btnCall");
    uiEnableEditBox("btnDecline");
}


function uiPrepareTCall()   // update UI according to incoming call
{
    uiEnableEditBox("displayPad");
    uiEnableEditBox("btnCall");
    uiEnableEditBox("btnDecline");
}


function uiPlayRingBackTone()
{
    dbgTrace("\t uiPlayRingBackTone");
    var ringbackTone = document.getElementById('ringback');
    ringbackTone.play();
}


function uiStopRingBackTone()
{
    dbgTrace("\t uiStopRingBackTone");
    var ringbackTone = document.getElementById('ringback');
    ringbackTone.pause();
}

function uiPlayAlertTone()
{
    dbgTrace("\t uiPlayAlertTone");
    var tone = document.getElementById('alert');
    tone.play();
}

function uiStopAlertTone()
{
    dbgTrace("\t uiStopAlertTone");
    var tone = document.getElementById('alert');
    tone.pause();
}
/***********************************************************************
**
** message definitions
**
***********************************************************************/
var MSG_TYPE = {
    REGISTER:   "REGISTER",
    INVITE:     "INVITE",    
    RESPONSE:   "RESPONSE",
    RELEASE:    "RELEASE",
    DTMF:       "DTMF"
};

registerMsg = {
    message:MSG_TYPE.REGISTER,
    server:"",
    name:"",
    password:""
};

inviteMsg = {
    message:MSG_TYPE.INVITE,
    clrNbr:"100",
    cldNbr:"101",
    iceufrag:"",
    icepwd:"",
    sdp:"",
    candidate:""
};

responseMsg = {
    message: MSG_TYPE.RESPONSE,
    code:200,
    callid:0,
    sdp:"",
    candidate:"",
    iceufrag:"",
    icepwd:""
};

releaseMsg = {
    message: MSG_TYPE.RELEASE,
    callid:0
};

dtmfMsg = {
    message: MSG_TYPE.DTMF,
    callid:0,
    dtmf:''
};

/***********************************************************************
**
** Debug functions
**
***********************************************************************/
function dbgTrace(str)
{
    console.log(str)
}

/***********************************************************************
**
** JSON functions
**
***********************************************************************/
function objToJsonStr(obj)
{
    return JSON.stringify(obj);
}

function jsonStrToObj(jsonStr)
{
    // There are lots of '\r\n' in SDP string, JSON will fail to parse
    // it, so we have to replace '\r\n' to '\\r\\n' before parse it.
    var data = jsonStr.replace(/\r\n/g, "\\r\\n");
    return JSON.parse(data);
}

/***********************************************************************
**
** UA states
**
***********************************************************************/
var UA_STATE = {
    UNREG:  0,
    IDLE:   1,
    O_CALLING:2,
    O_ALERT:  3,
    O_TALK:   4,
    T_CALLING:5,
    T_ALERT:  6,
    T_TALK:   7
};

function ccStateStr(state)
{
    switch(state)
    {
    case UA_STATE.UNREG:
        return "UNREG";
    
    case UA_STATE.IDLE:
        return "IDLE";
        
    case UA_STATE.O_CALLING:
        return "O_CALLING";
    
    case UA_STATE.O_ALERT:
        return "O_ALERT";
        
    case UA_STATE.O_TALK:
        return "O_TALK";
    
    case UA_STATE.T_CALLING:
        return "T_CALLING";
    
    case UA_STATE.T_ALERT:
        return "T_ALERT";
        
    case UA_STATE.T_TALK:
        return "T_TALK";
        
    default:
        return state;
    }
}

function ccEnterState(newState)
{
    dbgTrace("ccEnterState: " + ccStateStr(uaState) + " --> " + ccStateStr(newState));
    uaState = newState;
    
    switch(uaState)
    {
    case UA_STATE.IDLE:
        ccInitCallParams();
        uiPrepareIdle();
        break;
        
    case UA_STATE.UNREG:
        wsClose();
        break;
    
    default:
        break;
    }
}


/***********************************************************************
**
** media stream functions
**
***********************************************************************/                  
function wrtcStartAudioStream()
{
    dbgTrace("wrtcStartAudioStream");
    
    var mediaConfig = {video:false, audio:true}; 
    navigator.webkitGetUserMedia(mediaConfig,
                                 onMediaSuccess, onMediaError);
                                 
    function onMediaSuccess(mediaStream)
    {
        dbgTrace("onMediaSucess");
        localAudioStream = mediaStream;
        
        wsInit();      
    }
    
    function onMediaError(error)
    {
        dbgTrace("onMediaError");
        dbgTrace("\t " + error );
    }
}


/***********************************************************************
**
** Peer connection functions
**
***********************************************************************/
function wrtcStartPC()
{
    dbgTrace("wrtcStartPC");
    //var pcConfig={iceServers:[{url:"stun:stun.l.google.com:19302"}]};
    var pcConfig={iceServers:[{url:"stun:stun.minisipserver.com"}]};
    var pcOptional={optional:[{DtlsSrtpKeyAgreement: false}]};
    
    try {
        localCandidate = "";
        
        peerConnection = new webkitRTCPeerConnection(pcConfig, pcOptional); 
                      
        peerConnection.onicecandidate = function(event) {
            dbgTrace("\t PeerConnection onicecandidate");
            var c = event.candidate;
            if (c!=null)
            {
                dbgTrace("\t\t original candidate: "+objToJsonStr(c));
                var candidate = c.candidate;
                if(candidate.indexOf("udp")>0) // only care 'UDP' result
                {
                    localCandidate = candidate;
                    dbgTrace("\t\t " + candidate);
                }
            }
            else
            {
                // No more candidate, then get last record as result
                if(uaState==UA_STATE.O_CALLING)
                {
                    inviteMsg.clrNbr = mssConfig.user;
                    inviteMsg.cldNbr = $("#displayPad").val();
                    inviteMsg.sdp = localSDPInfo;
                    inviteMsg.candidate = localCandidate;
                    inviteMsg.iceufrag = localIceufrag;
                    inviteMsg.icepwd = localIcePwd;
                    wsSend(objToJsonStr(inviteMsg));
                }
                else if(uaState == UA_STATE.T_TALK)// for T-answer
                {
                    // send 200 to MSS to answer call
                    responseMsg.code = 200;
                    responseMsg.callid = mssCallID;
                    responseMsg.sdp = localSDPInfo;
                    responseMsg.candidate = localCandidate;
                    responseMsg.iceufrag = localIceufrag;
                    responseMsg.icepwd = localIcePwd;
                    wsSend(objToJsonStr(responseMsg));
                }
            }
        };
                
        peerConnection.onnegotiationneeded = function() {    
            dbgTrace("\t PeerConnection onnegotiationneeded");              
        }
        
        peerConnection.onaddstream = function(event) {
            // add remote stream
            dbgTrace("\t peerConnection.onaddstream");
            
            document.getElementById('remoteAudio').src = URL.createObjectURL(event.stream);
        }     
        
    }
    catch(e) {
        dbgTrace("fail to create PC: "+e.message);
        return;
    }
}

function wrtcStopPC()
{
    dbgTrace("wrtcStopPC");
    
    localAudioStream.stop();
           
    peerConnection.removeStream(localAudioStream);    
    peerConnection.close();         
}


function wrtcSetLocalSDP(localDesc)
{
    dbgTrace("\t wrtcSetLocalSDP");    
    peerConnection.setLocalDescription(localDesc);
    var sdp = localDesc.sdp;
    dbgTrace("\t\t local SDP: "+sdp);
    
    var sdpLines = sdp.split("\r\n");
    parserLocalSDP(sdpLines); 
}

/***********************************************************************
**
** Web socket functions
**
***********************************************************************/
function wsInit()
{
    dbgTrace("wsInit");
    dbgTrace("\t "+mssConfig.addr);    
    var wsAddr = "ws://"+mssConfig.addr+":"+mssConfig.port;
    wsSocket = new WebSocket(wsAddr);
    wsSocket.onopen = function() {
        dbgTrace("web socket opened.");    
        ccRegister();
    }; // wsSocket.onopen
    
    wsSocket.onclose=function() {
        dbgTrace("web socket close");
        wsClose();
    }; // wsSocket.onclose
    
    wsSocket.onmessage=function(evt) {
        dbgTrace("recv message from websocket ...");
        dbgTrace(evt.data);
        
        var msg = jsonStrToObj(evt.data);     
        ccRecvMessage(msg);
        
    }; // wsSocket.onmessage   
}

function wsClose()
{
    if(wsSocket==0)
    {
        return;
    }
    wsSocket.close();
    wsSocket = 0;
}

function wsSend(text)
{
    dbgTrace("wsSend: "+text);
    wsSocket.send(text);    
}


/***********************************************************************
**
** SDP functions
**
***********************************************************************/
function parserLocalSDP(sdpLines)
{
    dbgTrace("\t parserLocalSDP");
    for( var i=0; i<sdpLines.length; i++)
    {
        var sdp = sdpLines[i];
        // at this time, we only care 'AES_CM_128_HMAC_SHA1_80'.
        if( sdp.indexOf("AES_CM_128_HMAC_SHA1_80") > 0 )
        {
            localSDPInfo = sdp;
            return;
        }
        else if( sdp.indexOf("a=ice-ufrag:")>=0)
        {
            localIceufrag = sdp.slice(12); 
        }
        else if(sdp.indexOf("a=ice-pwd:")>=0)
        {
            localIcePwd = sdp.slice(10);
        }
        else
        {
            // do nothing
        }
    }
    dbgTrace("\t\t cannot find 'AES_CM_128_HMAC_SHA1_80'???"); 
}

/***********************************************************************
**
** register functions
**
***********************************************************************/
var statRegFailCnt = 0;
function buildMD5Password()
{
    return hex_md5(mssConfig.user+":"+nonce+":"+mssConfig.password);
}

function ccRegister()
{
    dbgTrace("\t ccRegister");
    registerMsg.server = mssConfig.addr;
    registerMsg.name = mssConfig.user;
    registerMsg.password = buildMD5Password();
    wsSend(objToJsonStr(registerMsg));
}

function ccRecvMessage(msg)
{
    dbgTrace("ccRecvMessage");
    if(msg.message == MSG_TYPE.RELEASE)
    {
        ccRecvRelease(msg);
        return;
    }
    
    switch(uaState)
    {
        case UA_STATE.UNREG:
            onStateUNREG(msg);
            break;
            
        case UA_STATE.IDLE:
            onStateIdle(msg);
            break;
            
        case UA_STATE.O_CALLING:
            onStateOCalling(msg);
            break;
        
        case UA_STATE.O_ALERT:
            onStateOAlert(msg);
            break;
            
        case UA_STATE.T_TALK:
            break;
            
        default:
            ccOAnswer(msg);
            break;
    }
}

function ccRecvRelease(msg)
{
    dbgTrace("\t ccRecvRelease");
    if(mssCallID==0 || mssCallID != msg.callid )
    {
        dbgTrace("\t\t unnecessary to release call now.");
        return;
    }
    
    uiStopAlertTone();
    uiStopRingBackTone();
    wrtcStopPC();
    
    ccEnterState(UA_STATE.IDLE);
}


function onStateUNREG(msg)
{
    dbgTrace("onStateUNREG");
    if(msg.message != MSG_TYPE.RESPONSE)
    {
        dbgTrace("\t unknown message: " + msg.message);
        return;
    }
    
    switch(msg.code)
    {
    case 401:
        statRegFailCnt++;
        if(statRegFailCnt>=3)
        {
            dbgTrace("\t\t fail to register to server.");
            return;
        }
        nonce = msg.nonce;
        ccRegister(); // register again with new authorization
        return;
    
    case 200:   // online now
        statRegFailCnt=0;
        ccEnterState(UA_STATE.IDLE);
        return;
        
    default:
        dbgTrace("\t unkwon code: "+msg.code);
    }
}


function onStateIdle(msg)
{
    dbgTrace("onStateIdle");
    if(msg.message == MSG_TYPE.INVITE)
    {
        ccTCall(msg);
    }
}

function onStateOCalling(msg)
{
    dbgTrace("onStateOCalling");
    if(msg.message == MSG_TYPE.RESPONSE)
    {
        mssCallID = msg.callid;
        
        oCalling_recvResp(msg);
    }
}

function onStateOAlert(msg)
{
    dbgTrace("onStateOAlert");
    if(msg.message == MSG_TYPE.RESPONSE)
    {
        oAlert_recvResp(msg);
    }
}

function oCalling_recvResp(msg)
{
    dbgTrace("\t oCalling_recvResp");
    
    switch(msg.code)
    {
    case 180:
        uiPlayRingBackTone();
        ccEnterState(UA_STATE.O_ALERT);
        break;
    
    case 200:
        uiStopRingBackTone();
        ccOAnswer(msg);
        ccEnterState(UA_STATE.O_TALK);
        break;
    }
    
    return;
}


function oAlert_recvResp(msg)
{
    dbgTrace("\t oAlert_recvResp");
    
    if(msg.code == 200)
    {
        uiStopRingBackTone();
        ccOAnswer(msg);
        ccEnterState(UA_STATE.O_TALK);
    }
}

/***********************************************************************
**
** calls functions
**
***********************************************************************/
function ccOCall()
{
    dbgTrace("\t ccOCall");    
    
    if(false == uiEditBoxHasVal("displayPad"))
    {
        return;
    }
    
    wrtcStartPC();
    peerConnection.addStream(localAudioStream);
    peerConnection.createOffer(wrtcSetLocalSDP);
    
    ccEnterState(UA_STATE.O_CALLING);
}


function ccOAnswer(msg)
{
    dbgTrace("\t ccOAnswer");
    
    var sdc = new RTCSessionDescription();
    sdc.sdp = msg.sdp;
    sdc.type = 'answer';
    
    peerConnection.setRemoteDescription(sdc);    
    peerConnection.addIceCandidate(new RTCIceCandidate({"sdpMLineIndex":0, "sdpMid":"audio","candidate":msg.candidate}));
}


function ccTCall(msg)
{
    dbgTrace("\t ccTCall");
    ccEnterState(UA_STATE.T_CALLING);
    
    mssCallID = msg.callid;
    mssSDPInfo = msg.sdp;
    mssCandidate = msg.candidate;
    
    uiDisplayIncomingCall(msg.clrNbr);
    
    wrtcStartPC();
    ccTAlert();
}

function ccTAlert()
{
    dbgTrace("\t ccTAlert");
    
    uiPrepareTCall();
    uiPlayAlertTone();
    
    responseMsg.code = 180;
    responseMsg.callid = mssCallID;
    wsSend(objToJsonStr(responseMsg));
                    
    ccEnterState(UA_STATE.T_ALERT);
}

function ccTAnswer()
{
    dbgTrace("\t ccTAnswer");
    
    peerConnection.addStream(localAudioStream);
    
    var sdc = new RTCSessionDescription();
    sdc.sdp = mssSDPInfo;
    sdc.type = 'offer';        
    peerConnection.setRemoteDescription(sdc);        
    peerConnection.addIceCandidate(new RTCIceCandidate({"sdpMLineIndex":0, "sdpMid":"audio","candidate":mssCandidate}));

    peerConnection.createAnswer(
        function(answerSDP) {
            dbgTrace("\t\t answer SDP: "+ answerSDP.sdp);       
            dbgTrace("\t\t SDP type: "+ answerSDP.type);
            peerConnection.setLocalDescription(answerSDP);               
    
            var sdp = answerSDP.sdp;
            var sdpLines = sdp.split("\r\n");
            parserLocalSDP(sdpLines); 
            
            ccEnterState(UA_STATE.T_TALK);
        }        
    );
}

function ccInitCallParams()
{
    mssCallID = 0;
    mssCandidate = "";
    mssSDPInfo = "";
}

function ccEndCall()
{
    dbgTrace("\t ccEndCall");
    ccSendRelease();
    
    ccEnterState(UA_STATE.IDLE);
}

function ccSendRelease()
{
    dbgTrace("\t ccSendRelease");
    if(mssCallID==0)
    {
        return;
    }
    
    releaseMsg.callid = mssCallID;
    wsSend(objToJsonStr(releaseMsg));
    mssCallID = 0;
}

function ccSendDTMF(digit)
{
    dbgTrace("\t ccSendDTMF");
    if(mssCallID==0)
    {
        dbgTrace("\t\t no call, unnecessary to send it");
        return;
    }
    
    if(uaState!=UA_STATE.O_TALK && uaState!=UA_STATE.T_TALK)
    {
        dbgTrace("\t\t invalid state, discard it.");
        return;
    }
    dbgTrace("\t\t digit="+digit);
    
    dtmfMsg.callid = mssCallID;
    dtmfMsg.dtmf = digit;
    wsSend(objToJsonStr(dtmfMsg));
}
