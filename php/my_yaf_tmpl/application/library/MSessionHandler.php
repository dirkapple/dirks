<?php
/**
 * MSessionHandler
 *
 * 重新设置Session的句柄，使用Memcached作为Session的存储方式，实现跨机器Session一致
 */
class MSessionHandler implements SessionHandlerInterface {
    private $_memcache;
    private $_expire;

    public function __construct(Memcache $memcache, $expire_time) {
        $this->_memcache = $memcache;
        $this->_expire = $expire_time;
    }

    public function open($savePath, $sessionName) {
        return true;
    }

    public function close() {
        return true;
    }

    public function read($sessionId) {
        $session = $this->_memcache->get($sessionId);
        return $session ? $session : '';
    }

    public function write($sessionId, $data) {
        if (!$this->_memcache->set($sessionId, $data, 0, $this->_expire)) {
            QLogger::log('SESSION.WRITE.FAILED', "$sessionId\t$data");
        }
        return true;
    }

    public function destroy($sessionId) {
        return $this->_memcache->delete($sessionId);
    }

    public function gc($lifetime) {
        return true;
    }
}
