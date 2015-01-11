<?php
/**
 * Created by JetBrains PhpStorm.
 * FileName: Activity.php
 * Author: libofeng
 * Date: 13-1-10
 * Time: 上午10:09
 */
class ActivityController extends Yaf_Controller_Abstract {

    /**
     * key data struct   : prize_day_FIRST_1
     * value data struct : status|prize|guid|time
     *       status: R ready W wait S succ
     *       prize : 1 2 3 4
     *       guid  : user ID
     * eg: R|1|cacao|138132456
     */

    private $_passwd = "WeC1khJ#V/%aryf5L&Lj&nk&]hcM?^GU@,2Z3'\"$7hCBjwotW3C\"plF,YA(w[Wba";
    private $_chance = 30000; // 预估抽奖次数
    private $_rconf = '123.125.80.17:63795';
    //private $_rconf = '127.0.0.1:6379';
    const PRIZE_HASH_NAME = '__PRIZE__';
    const PRIZE_TUAN_NAME = '__TUAN__';
    const PRIZE_KEY_COUNT = 'prize_REQUEST_count';

    private $_map = array(
        '17' => '_FIRST_',
        '19' => '_SECOND_',
        //'21' => '_THIRD_',
        '_17' => '1-1',
        '_19' => '11-13',
        //'_21' => '101-109',
        '17_' => 1,
        '19_' => 2,
        //'21_' => 3,
        );

    private $_desc = array(
        1 => '恭喜您获得由携程提供的韩国%s日游！提交以下信息会有工作人员在2周内与您联系哦！<br />请在<em>10分钟</em>之内提交您的信息',
        2 => '恭喜您获得由酷讯提供的千元机票红包！提交以下信息会有工作人员在2周内与您联系哦！<br />请在<em>10分钟</em>之内提交您的信息',
        3 => '恭喜您获得由%s提供的免费酒店1晚！提交以下信息会有工作人员在2周内与您联系哦！<br />请在<em>10分钟</em>之内提交您的信息',
        4 => '恭喜您获得<a href="http://tuan.360.cn/static/subject/nianhuo2013.html?fname=search_zhuanti_huodong&eee=search_zhuanti_huodong&fsign=12490dd23c" target="_blank">360特供年货</a>团购券<br />请在<em>10分钟</em>之内提交您的邮箱领取',
        );
    private $_pname = array(
        1 => '韩国%d日游',
        2 => '千元机票红包',
        3 => '免费酒店入住',
        );

    private $_vendor = array(
        14 => 'A|pname|http://test.com',
        15 => '麦兜团|5元团购券|http://www.maidoutuan.com',
        16 => '麦圈网|5元团购券|http://www.much001.com',
        17 => '猛买网|满200-10元团购券|http://www.mengmai.com',
        18 => 'Like团|满100-5元团购券|http://www.liketuan.com',
        19 => '欧乐库|5元团购券|http://www.ouleku.com',
        20 => '天秀团|满100-10元团购券|http://tuan.tx29.com',
        21 => '女鞋团|满100-5元团购券|http://www.nxtuan.cn',
        22 => '团购王|3元团购券|http://www.go.cn',
        23 => '团友网|满50-10元团购券|http://www.tuanu.com',
        24 => '合合买|满100-10元团购券|http://tuan.hehebuy.com',
        25 => '团乐购|满50-5元团购券|http://www.tuanlego.com',
        26 => '品质团|5元团购券|http://www.pztuan.com',
        27 => '拍鞋网|满100-20元团购券|http://tuan.paixie.net',
        28 => '麦兜团|满300-20元团购券|http://www.maidoutuan.com',
        );

    public function prizeAction() {

        $ymd = date('Ymd');
        if ($ymd < '20130114' || $ymd > '20130128') {
            echo $this->jout(-100, array('msg' => 'error date'));
            return false;
        }

        $ip = $_SERVER['REMOTE_ADDR'];
        $verify = $this->getRequest()->getCookie("prize_verify", "");
        if (sha1($ip . $this->_passwd) != $verify) {
            echo $this->jout(-50, array('msg' => 'error ip'));
            return false;
        }

        if (CheckSpider::check($ip)) {
            echo $this->jout(-2, array('msg' => 'ip limit'));
            return false;
        }

        $guid = $this->getRequest()->getCookie("QiHooGUID", "");
        if (empty($guid)) {
            echo $this->jout(-3, array('msg' => 'error refer'));
            return false;
        }

        Yaf_Loader::import('redislib/lib/Predis.php');
        list($host, $port) = explode(':', $this->_rconf);
        $redis = new Predis_Client(array('host' => $host, 'port' => $port)); 

        try {
            $prefix = 'prize_' . $ymd;
            $redis->incr(self::PRIZE_KEY_COUNT . '_' . $ymd);
            $count = $redis->get(self::PRIZE_KEY_COUNT . '_' . $ymd);
            !$count ? $count = 1 : true;

            if ($count < $this->_chance) {
                $roll = mt_rand(1, $this->_chance);
            } else if ($count == $this->_chance) {
                $roll = 1; // 第200w个中奖
            } else {
                $roll = mt_rand(1, $count);
            }

            if ($roll === 1) {
                $pos = 1;
                $key = $prefix . '_FIRST_1';
                $prize = $redis->get($key);
                if (!$prize) {
                    $redis->set($key, 'R|1|' . $guid . '|' . time());
                } 
            } else if ($roll > 10 && $roll < 14) {
                $pos = $roll - 10;
                $key = $prefix . '_SECOND_' . $pos;
                $prize = $redis->get($key);
                if (!$prize) {
                    $redis->set($key, 'R|2|' . $guid . '|' . time());
                }
            } 
            /*else if ($roll > 100 && $roll < 110) {
                $pos = $roll - 100;
                $key = $prefix . '_THIRD_' . $pos;
                $prize = $redis->get($key);
                if (!$prize) {
                    $redis->set($key, 'R|3|' . $guid . '|' . time());
                }
            } */
            else if ($roll > 10000 && $roll < 30001) {
                $pos = $roll - 10000;
                $key = $prefix . '_FORTH_' . $pos;
                $prize = $redis->get($key);
                if (!$prize) {
                    $redis->set($key, 'R|4|' . $guid . '|' . time());
                }
            } else {
                // 不中君的福音:奖必须发出去！
                $hour = date("G");
                if (isset($this->_map[$hour])) {
                    list($low, $high) = explode('-', $this->_map['_' . $hour]);
                    $roll = mt_rand($low, $high * 100);
                    if ($roll >= $low && $roll <= $high) {
                        if ($hour == 17) {
                            $pos = $roll;
                        } else if ($hour == 19) {
                            $pos = $roll - 10;
                        } else if ($hour == 21) {
                            $pos = $roll - 100;
                        } else {
                            echo $this->jout(-1);
                            return false;
                        }
                        $key = $prefix . $this->_map[$hour] . $pos;
                        $prize = $redis->get($key);
                        if (!$prize) {
                            $redis->set($key, 'R|'. $this->_map[$hour . '_'] .'|' . $guid . '|' . time());
                        }
                    }
                }
            }
        } catch (Exception $e) {
            echo $this->jout(-1);
            return false;
        }

        if (isset($key)) {
            try {
                $trans = $redis->multiExec();
                $trans->watch($key);
                $prize = $redis->get($key);
                $trans->multi();

                if (!$prize) {
                    echo $this->jout(-1);
                    return false;
                }

                if ($prize[0] === 'R') {
                    $parts = explode('|', $prize);
                    if (count($parts) != 4) {
                        $trans->del($key);
                        $trans->exec();
                        echo $this->jout(-1);
                        return false;
                    }

                    $parts[3] = time();
                    if ($parts[1] == 4) {
                        $parts[0] = 'S';
                        $trans->set($key, implode('|', $parts));
                    } else {
                        $parts[0] = 'W'; // status is waitting to complete user info now.
                        $trans->setex($key, 600, implode('|', $parts));
                    }
                    $trans->exec();
                    $prizeID = sha1($this->_passwd . $guid . $parts[1] . $pos . $_SERVER['REMOTE_ADDR']);
                    $desc = $this->_desc[$parts[1]];
                    $pname = '';
                    $day = date('j');
                    if ($parts[1] == 1) {
                        if ($day <= 21) {
                            $desc = sprintf($desc, 4);
                            $pname = sprintf($this->_pname[1], 4);
                        } else {
                            $desc = sprintf($desc, 5);
                            $pname = sprintf($this->_pname[1], 5);
                        }
                    } else if ($parts[1] == 2) {
                        $pname = $this->_pname[2];
                    } else if ($parts[1] == 3) {
                        $pname = $this->_pname[3];
                        if ($day <= 24) {
                            $desc = sprintf($desc, "携程");
                        } else {
                            $desc = sprintf($desc, "酷讯");
                        }
                    } else if ($parts[1] == 4) {
                        list($vendor, $pname, $url) = explode('|', $this->_vendor[$day]);
                        $tuanID = $redis->hget(self::PRIZE_TUAN_NAME . $ymd, $pos);
                        if ($tuanID) {
                            //$desc = sprintf($desc, $tuanID);
                        } else {
                            echo $this->jout(-1);
                            return false;
                        }
                    }
                    if ($parts[1] == 4) {
                        echo $this->jout(0, array('pid' => $prizeID, 'lv' => $parts[1], 'pos' => $pos, 'desc' => $desc, 'vendor' => $this->_vendor[$day], 'pname' => $pname, 'url' => $url));
                    } else {
                        echo $this->jout(0, array('pid' => $prizeID, 'lv' => $parts[1], 'pos' => $pos, 'desc' => $desc, 'pname' => $pname));
                    }
                    return false;
                } else {
                    $trans->exec();
                }
            } catch (Exception $e) {
                echo $this->jout(-1);
                return false;
            }
        }

        echo $this->jout(-1);
        return false;
    }

    private function jout($code, $params = array()) {
        $res = new stdclass();
        $res->res = $code;
        if (is_array($params) && count($params) > 0) {
            foreach($params as $key => $val) {
                $res->$key = $val;
            }
        }

        return json_encode($res);
    }

    public function emailAction() {

        $ymd = date('Ymd');
        if ($ymd < '20130114' || $ymd > '20130128') {
            echo $this->jout(-100, array('msg' => '活动已结束'));
            return false;
        }

        $ip = $_SERVER['REMOTE_ADDR'];
        $verify = $this->getRequest()->getCookie("prize_verify", "");
        if (sha1($ip . $this->_passwd) != $verify) {
            echo $this->jout(-50, array('msg' => 'IP错误'));
            return false;
        }

        if (CheckSpider::check($ip)) {
            echo $this->jout(-10, array('msg' => 'IP频次受限'));
            return false;
        }

        $guid = trim($this->getRequest()->getCookie("QiHooGUID", ''));
        $pid = trim($this->getRequest()->getCookie('prize_pid', ''));
        $lv = trim($this->getRequest()->getCookie('prize_lv', ''));
        $pos = trim($this->getRequest()->getCookie('prize_pos', ''));
        if (empty($pid) || empty($guid) || empty($lv) || empty ($pos)) {
            echo $this->jout(-1, array('msg' => '参数错误'));
            return false;
        }

        $prizeID = sha1($this->_passwd . $guid . $lv . $pos . $_SERVER['REMOTE_ADDR']);
        if ($pid != $prizeID) {
            echo $this->jout(-2, array('msg' => '非法奖项ID'));
            return false;
        }
        
        $prefix = 'prize_' . $ymd;
        $key = $prefix . '_FORTH_' . $pos;
        Yaf_Loader::import('redislib/lib/Predis.php');
        list($host, $port) = explode(':', $this->_rconf);
        $redis = new Predis_Client(array('host' => $host, 'port' => $port)); 

        $isSent = $redis->get('prize_email_' . $ymd . '_' . $pos);
        if ($isSent === 's') {
            echo $this->jout(0, array('msg' => '发送成功'));
            return false;
        }

        $prizeInfo = $redis->get($key);
        if (!$prizeInfo) {
            echo $this->jout(-20, array('msg' => '中奖纪录不存在'));
            return false;
        } else {
            $parts = explode('|', $prizeInfo);
            if (count($parts) != 4) {
                $redis->del($key);
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                echo $this->jout(-21, array('msg' => '系统繁忙'));
                return false;
            }

            $oldTime = intval($parts[3]);
            $now = time();
            if ($now - $oldTime > 600) {
                $redis->del($key);
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                echo $this->jout(-22, array('msg' => '已过期'));
                return false;
            }

        }

        $email = trim($this->getRequest()->getQuery('email', ''));
        if (empty($email) || strlen($email) > 255) {
            echo $this->jout(-3, array('msg' => 'Email格式错误'));
            return false;
        }

        if (preg_match("/^[a-zA-Z0-9\_\-\.\+]+@[a-zA-Z0-9\_\-]+(\.[a-zA-Z0-9\_\-]+)+$/", $email)) {

            Yaf_Loader::import('redislib/lib/Predis.php');
            list($host, $port) = explode(':', $this->_rconf);
            $redis = new Predis_Client(array('host' => $host, 'port' => $port)); 
            $length = $redis->hlen('prize_succ_record');
            !$length ? $length = 0 : true;

            $day = date('j');
            $res = true;
            if (file_exists(APPLICATION_PATH . '/application/resouce/' . $day . '.eml')) {
                $tpl = file_get_contents(APPLICATION_PATH . '/application/resouce/' . $day . '.eml');
                if (empty($tpl)) $res = false;
                else {
                    $tuanID = $redis->hget(self::PRIZE_TUAN_NAME . $ymd, $pos);
                    if (!$tuanID) {
                        echo $this->jout(-8, array('msg' => '获取奖品错误'));
                        return false;
                    }
                    list($vendor, $prize, ) = explode('|', $this->_vendor[$day]);
                    $tpl = sprintf($tpl, $vendor, $prize, $tuanID);
                    $res = mail($email, '=?utf-8?B?' . base64_encode('中奖通知') . '?=', $tpl, "Return-Path: service-so@ex.360.cn\r\nContent-Type: text/plain; charset=UTF-8\r\nFrom: =?utf-8?B?".base64_encode("360搜索"). "?=<service-so@ex.360.cn>\r\n");
                }
            } else {
                $res = false;
            }

            if ($res) {
                $redis->hset('prize_succ_record', 'succ_' . (++$length), implode('|', array('S', 4, $guid, time(), 'default', $email)));
                $redis->setex('prize_email_' . $ymd . '_' . $pos, 600, 's');
                echo $this->jout(0, array('msg' => '发送成功'));
                return false;
            } else {
                echo $this->jout(-4, array('msg' => '发送失败'));
                return false;
            }
        }

        echo $this->jout(-5, array('msg' => 'Email格式错误'));
        return false;
    }

    public function confirmAction() {

        $ymd = date('Ymd');
        if ($ymd < '20130114' || $ymd > '20130128') {
            echo $this->jout(-100, array('msg' => '活动已结束'));
            return false;
        }

        $ip = $_SERVER['REMOTE_ADDR'];
        $verify = $this->getRequest()->getCookie("prize_verify", "");
        if (sha1($ip . $this->_passwd) != $verify) {
            echo $this->jout(-50, array('msg' => 'IP错误'));
            return false;
        }

        if (CheckSpider::check($ip)) {
            echo $this->jout(-10, array('msg' => 'IP频次受限'));
            return false;
        }

        $phone = trim($this->getRequest()->getQuery('phone', ''));
        $name = trim($this->getRequest()->getQuery('name', ''));
        $guid = trim($this->getRequest()->getCookie("QiHooGUID", ''));


        $pid = trim($this->getRequest()->getCookie('prize_pid', ''));
        $lv = trim($this->getRequest()->getCookie('prize_lv', ''));
        $pos = trim($this->getRequest()->getCookie('prize_pos', ''));
        if (empty($phone) || empty($name) || empty($pid) || empty($guid) || empty($lv) || empty ($pos)) {
            echo $this->jout(-1, array('msg' => '参数错误'));
            return false;
        }

        if (strlen($phone) != 11 || strlen($name) > 18) {
            echo $this->jout(-1, array('msg' => '参数错误'));
            return false;
        }

        $phone = intval($phone);
        $prefix = 'prize_' . $ymd;
        $prizeID = sha1($this->_passwd . $guid . $lv . $pos . $_SERVER['REMOTE_ADDR']);
        if ($pid != $prizeID) {
            echo $this->jout(-2, array('msg' => '非法奖项ID'));
            return false;
        }

        $lv == 1 && $lv = '_FIRST_';
        $lv == 2 && $lv = '_SECOND_';
        $lv == 3 && $lv = '_THRID_';
        $lv == 4 && $lv = '_FORTH_';
        $key = $prefix . $lv . $pos;

        Yaf_Loader::import('redislib/lib/Predis.php');
        list($host, $port) = explode(':', $this->_rconf);
        $redis = new Predis_Client(array('host' => $host, 'port' => $port)); 

        try {
            $trans = $redis->multiExec();
            $trans->watch($key);
            $prize = $redis->get($key);
            $trans->multi();

            if (!$prize) {
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                $trans->exec();
                echo $this->jout(-3, array('msg' => '中奖纪录不存在'));
                return false;
            }
            if ($prize[0] !== 'W') {
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                $trans->exec();
                echo $this->jout(-4, array('msg' => '中奖信息已填写'));
                return false;
            }

            $parts = explode('|', $prize);
            if (count($parts) != 4) {
                $trans->del($key);
                $trans->exec();
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                echo $this->jout(-5, array('msg' => '系统繁忙'));
                return false;
            }

            $oldTime = intval($parts[3]);
            $now = time();
            if ($now - $oldTime > 600) {
                $trans->del($key);
                $trans->exec();
                setcookie('prize_lv', null);
                setcookie('prize_pos', null);
                setcookie('prize_pid', null);
                echo $this->jout(-6, array('msg' => '已过期'));
                return false;
            }

            $parts[0] = 'S';
            $parts[] = $name;
            $parts[] = $phone;
            $trans->set($key, implode('|', $parts));
            $trans->exec();
            // for 中奖展示  四等奖 已经置为S
            $length = $redis->hlen('prize_succ_record');
            !$length ? $length = 0 : true;
            $redis->hset('prize_succ_record', 'succ_' . (++$length), implode('|', $parts));
            // searialize
            echo $this->jout(0, array('msg' => '提交成功'));
            return false;
        } catch (Exception $e) {
            echo $this->jout(-7, array('msg' => '系统繁忙！'));
            return false;
        }
    }

    public function luckyOnesAction() {
        $ymd = date('Ymd');
        if ($ymd < '20130114' || $ymd > '20130128') {
            echo $this->jout(-100, array('msg' => 'error date'));
            return false;
        }

        $ip = $_SERVER['REMOTE_ADDR'];
        $verify = $this->getRequest()->getCookie("prize_verify", "");
        if (sha1($ip . $this->_passwd) != $verify) {
            echo $this->jout(-50, array('msg' => 'error ip'));
            return false;
        }

        if (CheckSpider::check($ip)) {
            echo $this->jout(-10, array('msg' => 'ip limit'));
            return false;
        }

        $pos = intval($this->getRequest()->getQuery('pos', 1));
        $num = intval($this->getRequest()->getQuery('num', 10));
        $num > 30 ? $num = 10 : true;
        Yaf_Loader::import('redislib/lib/Predis.php');
        list($host, $port) = explode(':', $this->_rconf);
        $redis = new Predis_Client(array('host' => $host, 'port' => $port)); 
        $hlen = $redis->hlen('prize_succ_record'); 
        $total = 0;
        $records = array();
        $day = date('j');
        $loop = false;

        // second tmp
        $secTmp = null;
        $uniq = array();
        if ($hlen && $hlen > 0) {
            if ($pos > $hlen)
                $pos = $pos % $hlen;

            $pos <= 0 ? $pos = 1 : true;
            $ori = $pos;
            for($i = 0; $i < $num; $i++) {
                $pos += 1;
                if ($pos > $hlen) {
                    if (!$loop) {
                        $loop = true;
                        $pos = 1;
                    } else {
                        break;
                    }
                }

                if ($loop && $pos > $ori) {
                    break;
                }

                if ($i === 0) {
                    $first = $redis->keys('prize_*_FIRST_1');
                    if (($carr = count($first)) > 0) {
                        $idx = mt_rand(0, $carr - 1);
                        $fval = $redis->get($first[$idx]);
                        if ($fval) {
                            $record = $fval;
                            $uniq[] = $fval;
                        }
                    }
                } else if ($i === 1 || $i === 2) {
                    $second = $redis->keys('prize_*_SECOND_*');
                    if (($carr = count($second)) > 0) {
                        if ($secTmp === null) $idx = mt_rand(0, $carr - 1);
                        else {
                            if (isset($second[$secTmp + 1])) $idx = $secTmp + 1;
                            else if (isset($second[$secTmp - 1])) $idx = $secTmp - 1; 
                            else $idx = -100;
                        }
                        if ($idx >= 0)
                            $sval = $redis->get($second[$idx]);
                            if ($sval) {
                                $record = $sval;
                                $uniq[] = $fval;
                            }
                    }
                } else {
                    $record = $redis->hget('prize_succ_record', 'succ_' . $pos);
                }

                if (!$record) {
                    continue;
                } else if ($i > 2 && in_array($record, $uniq)) {
                    continue;
                } else {
                    $arr = explode('|', $record);
                    if (count($arr) != 6) {
                        continue;
                    } else {
                        $info = '';
                        if ((strpos($arr[5], '@')) !== false) {
                            list($vendor, $pname) = explode('|', $this->_vendor[$day]);
                            list($start, $end) = explode('@', $arr[5]);
                            if (strlen($start) <= 3) {
                                $start[1] = '*';
                            } else {
                                $start[2] = $start[3] = $start[4] = '*';
                            }
                            $info = $start . '@' . $end;
                        } else {
                            $pname = $this->_pname[$arr[1]];
                            if ($arr[1] == 1) { 
                                if($day <= 21) {
                                    $pname = sprintf($pname, 4);
                                } else {
                                    $pname = sprintf($pname, 5);
                                }
                            }
                            $info = $arr[5];
                            $info[3] = $info[4] = $info[5] = $info[6] = '*';
                        }
                        $records[] = array('prize' => $pname, 'info' => $info);
                        $total++;
                    }
                }
            }

            echo $this->jout(0, array('records' => $records, 'total' => $total));
            return false;
        } else {
            echo $this->jout(-1, array('msg' => 'record error'));
            return false;
        }

    }
}
