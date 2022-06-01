<?php
require_once("../inc/init.inc.php");
if(!internauteEstConnecteEtEstAdmin())
{
	header("location:../connexion.php");
	exit();
}

if(isset($_GET['msg']) && $_GET['msg'] == "update")
{
	executeRequete("UPDATE commande SET etat = etat + 1 WHERE id_commande=$_GET[id_commande]");
	header("location:gestion_commande.php");
}

if(isset($_GET['msg']) && $_GET['msg'] == "annuler")
{
	executeRequete("UPDATE commande SET etat = 5 WHERE id_commande=$_GET[id_commande]");
	header("location:gestion_commande.php");
}
//-------------------------------------------------- Affichage ---------------------------------------------------------//
	require_once("../inc/haut.inc.php");
	echo '<h1> Voici les commandes passées sur le site </h1>';
	echo '<table border="1"><tr>';
	
	$information_sur_les_commandes = executeRequete("SELECT commande.*, client.nom,
	client.prenom, client.email, client.adresse, client.ville, client.code_postal FROM commande left join client on  client.id_client = commande.id_client");
	echo "Nombre de commande(s) : " . $information_sur_les_commandes->num_rows;
	echo "<table style='border-color:red' border=10> <tr>";
	while($colonne = $information_sur_les_commandes->fetch_field())
	{    
		echo '<th>' . $colonne->name . '</th>';
	}
	echo '<th> Modifier </th>';
	echo '<th> Annulation </th>';
	echo "</tr>";
	$chiffre_affaire = 0;
	while ($commande = $information_sur_les_commandes->fetch_assoc())
	{
		$chiffre_affaire += $commande['montant'];
		echo '<div>';
		echo '<tr>';
		echo '<td><a href="gestion_commande.php?suivi=' . $commande['id_commande'] . '">Voir la commande ' . $commande['id_commande'] . '</a></td>';
		echo '<td>' . $commande['id_client'] . '</td>';
		echo '<td>' . $commande['montant'] . '</td>';
		echo '<td>' . $commande['date_enregistrement'] . '</td>';
		echo '<td>' . $commande['etat'] . '</td>';
		echo '<td>' . $commande['nom'] . '</td>';
		echo '<td>' . $commande['prenom'] . '</td>';
		echo '<td>' . $commande['email'] . '</td>';
		echo '<td>' . $commande['adresse'] . '</td>';
		echo '<td>' . $commande['ville'] . '</td>';
		echo '<td>' . $commande['code_postal'] . '</td>';
		switch ($commande['etat']) {
			case 'en cours de traitement':
				echo "<td><a href='gestion_commande.php?msg=update&&id_commande=" . $commande['id_commande'] . "' onclick='return(confirm(\"Etes-vous sùr de vouloir confirmer cette commande?\"));'> Confirmer la commande </a></td>";
				break;
			
			case 'confirmé':
				echo "<td><a href='gestion_commande.php?msg=update&&id_commande=" . $commande['id_commande'] . "' onclick='return(confirm(\"Etes-vous sùr de vouloir confirmer cette commande?\"));'> Commande envoyée </a></td>";
				break;

			case 'envoyé':
				echo "<td><a href='gestion_commande.php?msg=update&&id_commande=" . $commande['id_commande'] . "' onclick='return(confirm(\"Etes-vous sùr de vouloir confirmer cette commande?\"));'> Commande livrée </a></td>";
				break;

			case 'livré':
				echo '<td> Livrée </td>';
				break;
			
			case 'annulée':
				echo "<td> Commande annulée </td>";
				break;
			
			default:
				
				break;
		}
		if ($commande['etat'] != 'annulée')
		echo "<td><a href='gestion_commande.php?msg=annuler&&id_commande=" . $commande['id_commande'] . "' onclick='return(confirm(\"Etes-vous sure de vouloir annuler cette commande?\"));'> Annuler </a></td>";
		else
		echo "<td> Commande annulée </td>";
		echo '</tr>	';
		echo '</div>';
	}
	echo '</table><br />';
	echo 'Calcul du montant total des revenus:  <br />';
		print "le chiffre d'affaires de la societe est de : $chiffre_affaire €"; 
	
	echo '<br />';
	if(isset($_GET['suivi']))
	{	
		echo '<h1> Voici le détails pour une commande</h1>';
		echo '<table border="1">';
		echo '<tr>';
		$information_sur_une_commande = executeRequete("SELECT * FROM details_commande WHERE id_commande=$_GET[suivi]");
		
		$nbcol = $information_sur_une_commande->field_count;
		echo "<table style='border-color:red' border=10> <tr>";
		for ($i=0; $i < $nbcol; $i++)
		{    
			$colonne = $information_sur_une_commande->fetch_field(); 
			echo '<th>' . $colonne->name . '</th>';
		}
		echo "</tr>";

		while ($details_commande = $information_sur_une_commande->fetch_assoc())
		{
			echo '<tr>';
				echo '<td>' . $details_commande['id_details_commande'] . '</td>';
				echo '<td>' . $details_commande['id_commande'] . '</td>';
				echo '<td>' . $details_commande['id_produit'] . '</td>';
				echo '<td>' . $details_commande['quantite'] . '</td>';
				echo '<td>' . $details_commande['prix'] . '</td>';
			echo '</tr>';
		}
		echo '</table>';
	}