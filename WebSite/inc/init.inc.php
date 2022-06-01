<?php
//--------- BDD
$mysqli = new mysqli("localhost", "akli", "22041999", "site");
if ($mysqli->connect_error) die('Un probléme est survenu lors de la tentative de connexion à la BDD : ' . $mysqli->connect_error);
// $mysqli->set_charset("utf8");
 
//--------- SESSION
session_start();

//--------- CHEMIN
define("RACINE_SITE","/site_web/");
 
//--------- VARIABLES
$contenu = '';
 
//--------- AUTRES INCLUSIONS
require_once("fonction.inc.php");
require_once ("dompdf/autoload.inc.php");