<?php
require 'sproto.php';
$sproto=new Sproto();
$sproto->load_proto_from_file('s2c', 's2c.spb');
$sproto->load_proto_from_file('c2s', 'c2s.spb');
$sproto->dumpproto('s2c');
$sproto->dumpproto('c2s');

$buffer=$sproto->encode('c2s', 'get.request', array('what'=>'want_some_key'));
$ret=$sproto->decode('c2s', 'get.request', $buffer);
var_dump($ret);
//make request 
//notice the request session id must be the same with the response session id
$reqbuffer=$sproto->request('c2s', 'get', array('what'=>'wanted'),10001);
$ret=$sproto->dispath('c2s', $reqbuffer);
var_dump($ret);
//make response
//notice the response session id must be the same with the request session id
$resbuffer=$sproto->response('c2s', 'get', array('result'=>'wanted_is_here'),10001);
$ret=$sproto->dispath('c2s', $resbuffer);
var_dump($ret);
        
        
        
        