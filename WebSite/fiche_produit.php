<?php

use Dompdf\Dompdf;
use Dompdf\Options;
require_once("inc/init.inc.php");
$html = "";
if(isset($_GET['msg']) && $_GET['msg'] == "getPDF")
{
	$res = executeRequete("SELECT * FROM produit WHERE id_produit = '$_GET[id_produit]'");
	
	$produit = $res->fetch_assoc();

	$src = explode('/', $produit['photo']);

	$html .= "<h1> Fiche produit : $produit[titre] </h1> </br></br>";

	$html .= "<strong> Reference : </strong> $produit[reference] <br/><br/>";
	
	$html .= "<img src='".$src[2]."/".$src[3]."'  width='150' height='150' border='1px' /><br/><br/>";
	$html .= "<strong>Prix : </strong> $produit[prix] <br/><br/>";
	$html .= "<strong>Description : </strong> $produit[description]";



	$options = new Options();	
	$options->set ('defaultFont', 'Courier');
	$options->set ('chroot', realpath(''));
	$dompdf = new Dompdf($options);

	$dompdf->loadHtml ($html);
	$dompdf->render();
	$fichier = $produit['titre'].'.pdf';
	$dompdf->stream($fichier);
}
//--------------------------------- TRAITEMENTS PHP ---------------------------------//
if(isset($_GET['id_produit'])) 	{ $resultat = executeRequete("SELECT * FROM produit WHERE id_produit = '$_GET[id_produit]'"); }
if($resultat->num_rows <= 0) { header("location:boutique.php"); exit(); }

$produit = $resultat->fetch_assoc();
$contenu .= "<div class='fiche_produit'><h2>Titre : $produit[titre]</h2><hr /><br />";
$contenu .= "<p>Categorie : $produit[categorie]</p>";
$contenu .= "<p>Couleur : $produit[couleur]</p>";
$contenu .= "<p>Taille : $produit[taille]</p>";
$contenu .= "<img src='$produit[photo]' width='150' height='150' />";
$contenu .= "<p><i>Description : $produit[description]</i></p><br />";
$contenu .= "<p>Prix : $produit[prix] €</p><br />";

if($produit['stock'] > 0)
{
	$contenu .= "<i>Nombre d'produit(s) disponible : $produit[stock] </i><br /><br />";
	$contenu .= '<form method="post" action="panier.php">';
		$contenu .= "<input type='hidden' name='id_produit' value='$produit[id_produit]' />";
		$contenu .= '<label for="quantite">Quantité : </label>';
		$contenu .= '<select id="quantite" name="quantite">';
			for($i = 1; $i <= $produit['stock'] && $i <= 5; $i++)
			{
				$contenu .= "<option>$i</option>";
			}
		$contenu .= '</select>';
		$contenu .= '<input type="submit" name="ajout_panier" value="ajout au panier" />';
	$contenu .= '</form>';
}
else
{
	$contenu .= 'Rupture de stock !';
}
$contenu .= "<br /><a href='boutique.php?categorie=" . $produit['categorie'] . "'>Retour vers la séléction de " . $produit['categorie'] . "</a>";

$contenu .= "<br/><a href='fiche_produit.php?msg=getPDF&&id_produit=" . $produit['id_produit'] . "' onclick='return(confirm(\"Etes-vous sure de vouloir télécharger la fiche produit en PDF?\"));'> <button>  Télécharger en PDF  </button> </a> </div>";
//--------------------------------- AFFICHAGE HTML ---------------------------------//
require_once("inc/haut.inc.php");

echo $contenu;


require_once("inc/bas.inc.php");