<?php
class Fate {

	private $generals = array();
	private $fateName = null;
	private $fateEffect = null;

	public function __construct() {
	}

	public function addGeneral($general) {
		$this->generals[] = $general;
	}

	public function setFateEffect() {
	}
}
