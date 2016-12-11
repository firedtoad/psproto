<?php
if(!extension_loaded('sproto'))
{
    exit("need sproto extension");
}
if(!function_exists('sp_newproto'))
{
    /**
     * this functions disable the ide warnings
     */
    function sp_newproto(){}
    function sp_newproto_from_file(){}
    function sp_type(){}
    function sp_pack(){}
    function sp_unpack(){}
    function sp_encode(){}
    function sp_decode(){}
    function sp_protocol(){}
    function sp_dumpproto(){}
    
}
class Sproto{
    /**
     * the sproto resources
     * @var array
     */
    private $sprotos=array();
    /**
     * sessions for request and response
     * @var array
     */
    private $sessions=array();
    //TYPE FOR  REQUEST AND RESPONSE
    const REQUEST=0;
    const RESPONSE=1;
    public function __construct()
    {
        
    }
    /**
     * load proto from binary string
     * @param string $pname this sproto resource name
     * @param string $buffer contains the binary string
     * @return void
     */
    public function load_proto_from_buffer(string $pname,string $buffer)
    {
        $sp=sp_newproto($buffer);
        if($sp)
        {
            $this->sprotos[$pname]=$sp;
        }
    }
    /**
     * load proto from binary file
     * @param string $pname this sproto resource name
     * @param string $file binary file path
     * @return void
     */
    public function load_proto_from_file(string $pname,string $file)
    {
        $sp=sp_newproto_from_file($file);
        if($sp)
        {
            $this->sprotos[$pname]=$sp;
        }
    }
    /**
     * dump the loaded proto
     * @param string $pname the resource name passe to load_proto_from_buffer or load_proto_from_file
     * @return void
     */
    public function dumpproto(string $pname)
    {
       if(!empty($this->sprotos[$pname]))
        {
            $ret=sp_dumpproto($this->sprotos[$pname]); 
        }
    }
    /**
     * pack encoded buffer
     * @param string $buffer
     * @return string
     */
    public static function pack(string $buffer):string
    {
        return sp_pack($buffer);
    }
    
    /**
     * query the sproto_type here no need proto resource name
     * @param string $type   
     * @return resource sproto_type
     */
    public function sp_type(string $type)
    {
        return sp_type($type);
    }
    
    /**
     * unpack encoded buffer
     * @param string $buffer
     * @return string
     */
    public static function unpack(string $buffer):string
    {
        return sp_unpack($buffer);
    }
    /**
     * encode proto by type 
     * @param string $pname 
     * @param mixed $type name or sproto_type return by sp_type
     * @param array $arr
     * @return string
     */
    public function encode(string $pname,$type, array $arr)
    {
        $ret='';
        if(!empty($this->sprotos[$pname]))
        {
            if(is_string($type))
            {
                $type=sp_type($this->sprotos[$pname],$type);
              
            }
            $ret=sp_encode($type,$arr); 
        }
        return $ret;
    }
    /**
     * decode proto by type 
     * @param string $pname
     * @param mixed $type name or sproto_type return by sp_type
     * @param string $buffer
     * @return array(array,int) first is the result seconde is decoded length
     */
    public function decode(string $pname,$type, string $buffer)
    {
        $ret=array();
        if(!empty($this->sprotos[$pname]))
        {
            if(is_string($type))
            {
                $type=sp_type($this->sprotos[$pname],$type);
            }
            $ret=sp_decode($type,$buffer);
        }
        return $ret;
    }    
    /**
     * get the proto 
     * @param string $pname
     * @param mixed $name_or_id  string or tagid 
     * @return array(tagid,$request_type,$response_type)
     */
    public function protocal(string $pname,$name_or_id)
    {
        $ret=array();
        if(!empty($this->sprotos[$pname]))
        {
            var_dump($this->sprotos[$pname]);
            $ret=sp_protocol($this->sprotos[$pname],$name_or_id);
        }
        return $ret;
    }

    /**
     * make request 
     * @param string $pname
     * @param mixed $proto string or tagid 
     * @param array $args the array contains
     * @param number $session
     * @param bool $packed  
     * @return string
     */
    public function request(string $pname,$proto,array $args,int $session=0,bool $packed=FALSE)
    {
        var_dump($pname);
        var_dump($proto);
        $protocal=$this->protocal($pname, $proto);
        $header=$this->encode($pname, 'package', array('type'=>$protocal[0],'session'=>$session));
        $content='';
        if(!empty($protocal[1]))
        {
            $content=$this->encode($pname, $protocal[1], $args);
        }
        $protoid=$protocal[0]?$protocal[0]:0;
        $protoname=!empty($protocal[3])?$protocal[3]:'';
        $response=$protocal[2]?$protocal[2]:TRUE;
        $sarr=array($protoid,$protoname,$response);
        $this->sessions[$session]=$sarr;
        return $packed?self::pack($header.$content):$header.$content;
    }
    
    /**
     * make response
     * @param string $pname 
     * @param mixed $proto string or tagid 
     * @param array $args
     * @param number $session
     * @param bool $packed 
     * @return string
     */
    public function response(string $pname,$proto,array $args,$session=0,bool $packed=FALSE)
    {
        $protocal=$this->protocal($pname, $proto);
        $header=$this->encode($pname, 'package', array('type'=>0,'session'=>$session));
        $content='';
        if(!empty($protocal[2]))
        {
            $content=$this->encode($pname, $protocal[2], $args);
        }
        return $packed?self::pack($header.$content):$header.$content;
    }
    /**
     * dispatch the net package 
     * @param string $pname proto resource name
     * @param string $buffer the unpacked buffer
     * @param string $header the protocal header name normally 'package'
     * @return array('type'=>self:REQUEST|self::RESPONSE,'msg'=>array());
     */
    public function dispath(string $pname,string $buffer,string $header='package')
    {
        $ret=array();
        $header=$this->decode($pname, $header, $buffer);
        $type=!empty($header[0]['type'])?$header[0]['type']:0;
        $session=!empty($header[0]['session'])?$header[0]['session']:0;
        $buffer=substr($buffer, $header[1]);
        if(!empty($type))
        {
            //request
            $protocal=$this->protocal($pname, $type);
            $res=NULL;
            $req=NULL;
            if(!empty($protocal[1]))
            {
                $req=$this->decode($pname, $protocal[1], $buffer);
                $res_header=NULL;
                if($session&&!empty($protocal[2]))
                {
                     $res_header=array('type'=>$type,'session'=>$session);
                     $res=$this->encode($pname, $protocal[2], $res_header);
                }
                $ret=array('type'=>self::REQUEST,'name'=>$protocal[3],'type'=>$type,'msg'=>$req[0],'session'=>$session,'res'=>$res);
            }
        }else{
            //response
            if(isset($this->sessions[$session][0])&&isset($this->sessions[$session][1])
                &&isset($this->sessions[$session][2]))
            {
                $id=$this->sessions[$session][0];
                $name=$this->sessions[$session][1];
                $res=$this->sessions[$session][2];
                unset($this->sessions[$session]);
                $rtres=array(array());
                if(is_resource($res))
                {
                    $rtres=$this->decode($pname, $res, $buffer);
                }
                $ret=array('type'=>self::RESPONSE,'name'=>$name,'id'=>$id,'session'=>$session,'msg'=>$rtres[0]);
            }
        }
        return $ret;
    }
    public static function netpack($buffer)
    {
        $packed=self::pack($buffer);
        $header_len=strlen($packed);
        return pack('n',$header_len).$packed;
    }
}
