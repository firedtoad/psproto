<?php
require 'sproto.php';
$sproto=new Sproto();
$sproto->load_proto_from_file('s2c', 's2c.spb');
$sproto->load_proto_from_file('c2s', 'c2s.spb');
$e=0;$t=0;
$send_session=1;
//assume skynet gate server listen on 127.0.0.1:8888
$s=fsockopen('127.0.0.1',8888,$e,$t,1000);
stream_set_blocking($s, 0);
//send the handshake to server
$hand=$sproto->request('c2s', 'handshake', array(),$send_session++);
fwrite($s, Sproto::netpack($hand));
//set something 
$hand=$sproto->request('c2s', 'set', array('what'=>'hello','value'=>'hello from c programmer dietoad'),10001);
fwrite($s, Sproto::netpack($hand));
$hand=$sproto->request('c2s', 'set', array('what'=>'yes','value'=>'yes message from php'),$send_session++);
fwrite($s, Sproto::netpack($hand));
// stream_set_blocking(STDIN,0);
while(1)
{
    $len=array();
    $header=fread($s, 2);
    if($s&&$header)
    {
        //header size is a 2bytes big endian unsigned short  
        $len=unpack('nlen', $header);
        var_dump($len);
    }
    if(!empty($len['len']))
    {
        $len=$len['len'];
        $left=$len;
        $readed=0;
        $r='';
        $buffer='';
        //read until we have enough bytes 
        while($readed!=$len)
        {
            $r=fread($s, $left);
            $buffer.=$r;
            $read_len=strlen($r);
            $readed+=$read_len;
            $left-=$read_len;
        }
        //unpacked the netpackage receive from the server
        $unpacked=Sproto::unpack($buffer);
        $disp=$sproto->dispath('c2s',$unpacked);
        print_r($disp);
        $receive_session=!empty($disp['session'])?$disp['session']:0;
        if($receive_session==($send_session-1))
        {
            echo "type hello or yes and press enter\n";
            $cx=fgets(STDIN);
            if($cx)
            {
                $reqbuffer=$sproto->request('c2s','get',array('what'=>trim($cx)), $send_session++);
                echo strlen($reqbuffer),"\n";
                fwrite($s, Sproto::netpack($reqbuffer));
            }
        }
       
    }
    usleep(100000);
}
