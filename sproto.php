<?php
if(extension_loaded('sproto'))
{
    var_dump($sp=sp_newproto_from_file('hi.spb'));
    sp_dumpproto($sp);
    $str='080000000300020019000000aa010000';
    $dt=pack('H*',$str);
    $pk=sp_pack($dt);
    var_dump($str);
    var_dump(unpack('H*', $pk));
    $str=str_repeat('8a', 30);
    $dt=pack('H*',$str);
    $pk=sp_pack($dt);
    var_dump($str);
    var_dump(unpack('H*', $pk));
    $spt=sp_querytype($sp,1,1);
    var_dump($spt);
    $tp=sp_type($sp,'handshake.response');
    var_dump($tp);
    $en=sp_encode($tp,'',array('msg'=>'hi'));
    var_dump($en);
}
