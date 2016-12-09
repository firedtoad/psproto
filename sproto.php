<?php
if(!extension_loaded('sproto'))
{
    exit("need sproto extension");
}
if(!function_exists('sp_newproto'))
{
    function sp_newproto(){}
    function sp_newproto_from_file(){}
    function sp_pack(){}
    function sp_unpack(){}
    function sp_encode_type(){}
    function sp_decode_type(){}
    function sp_encode_proto(){}
    function sp_decode_proto(){}
    function sp_dumpproto(){}
    
}
class Sproto{
    private $sprotos=array();
    public function __construct()
    {
        
    }
    public function load_proto_from_buffer(string $pname,string $buffer)
    {
        $sp=sp_newproto($buffer);
        if($sp)
        {
            $this->sprotos[$pname]=$sp;
        }
    }
    public function load_proto_from_file(string $pname,string $file)
    {
        $sp=sp_newproto_from_file($file);
        if($sp)
        {
            $this->sprotos[$pname]=$sp;
        }
    }
    
    public function dumpproto(string $pname)
    {
       if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_dumpproto($this->sprotos[$pname]); 
        }
    }
    
    public static function pack(string $buffer):string
    {
        return sp_pack($buffer);
    }
    
    public static function unpack(string $buffer):string
    {
        return sp_unpack($buffer);
    }
    
    public function encode_type(string $pname,string $type, array $arr)
    {
        $ret='';
        if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_encode_type($this->sprotos[$pname],$type,$arr); 
        }
        return $ret;
    }
    
    public function decode_type(string $pname,string $type, string $buffer)
    {
        $ret=array();
        if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_decode_type($this->sprotos[$pname],$type,$buffer);
        }
        return $ret;
    }
    public function encode_proto(string $pname,int $proto,int $what, array $arr)
    {
        $ret='';
        if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_encode_proto($this->sprotos[$pname],$proto,$what,$arr); 
        }
        return $ret;
    }
    
    public function decode_proto(string $pname,int $proto,int $what, string $buffer)
    {
        $ret=array();
        if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_decode_proto($this->sprotos[$pname],$proto,$what,$buffer);
        }
        return $ret;
    }
    
}

// if(extension_loaded('sproto'))
// {
    
//     var_dump($sp=sp_newproto_from_file('hi.spb'));
//     sp_dumpproto($sp);
//     $str='080000000300020019000000aa010000';
//     $dt=pack('H*',$str);
//     $pk=sp_pack($dt);
//     var_dump($str);
//     var_dump(unpack('H*', $pk));
//     $str=str_repeat('8a', 30);
//     $dt=pack('H*',$str);
//     $pk=sp_pack($dt);
//     var_dump($str);
//     var_dump(unpack('H*', $pk));
//     $spt=sp_querytype($sp,1,1);
//     var_dump($spt);
//     $tp=sp_type($sp,'handshake.response');
//     var_dump($tp);
//     $en=sp_encode($sp,'handshake.response',array('msg'=>'godlike'));
//     var_dump($en);
//     $msg=sp_decode($sp,'handshake.response',$en);
//     var_dump($msg);
// }


// $e=0;$t=0;
// $s=fsockopen('127.0.0.1',8800,$e,$t,1000);
// stream_set_blocking($s, 0);
// echo chr(7);
// while(1)
// {
//     $len=array();
//     $r=fread($s, 2);
//     if($s&&$r)
//     {
//         $len=unpack('nlen', $r);
//         print_r($len);
//     }
//     if(!empty($len['len']))
//     {
         
//     }
//     usleep(100000);
// }

$sproto=new Sproto();
$sproto->load_proto_from_file('s2c', 's2c.spb');
$sproto->load_proto_from_file('c2s', 'c2s.spb');
$sproto->dumpproto('s2c');
$sproto->dumpproto('c2s');
// $en=$sproto->encode_type('c2s', 'get.request', array('what'=>'arraasda'));

// var_dump($en);
// $en=$sproto->encode_type('s2c', 'package', array('type'=>1,'session'=>1));
// var_dump($en);
// $pk=$sproto->decode_type('s2c', 'package',$en);
// var_dump($pk);


// $ret=$sproto->decode('s2c', 'heartbeat', $en);
// print_r($ret);

// $en=$sproto->encode_proto('c2s', 1, 1, array('msg'=>'asdasdasd'));
// var_dump($en);
// $proto=$sproto->decode_proto('c2s', 1, 1, $en);
// var_dump($proto);
$en=$sproto->encode_proto('s2c', 1, 0, array());
var_dump($en);
$proto=$sproto->decode_proto('s2c', 1, 0, $en);
var_dump($proto);