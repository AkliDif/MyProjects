<?php
require_once("inc/init.inc.php");
if($_POST)
{
	//debug($_POST);
	if(empty($contenu)) 
	{
		$membre = executeRequete("SELECT * FROM client WHERE email='$_POST[email]'"); 
		if($membre->num_rows > 0)
		{
			$contenu .= "<div class='erreur'>Un compte avec cette adresse mail existe dejà,<a href=.\connexion.php\"><u>Cliquez ici pour vous connecter</u></a> ou choisissez une autre adresse mail.</div>";
		}
		elseif ($_POST['mdp_conf'] != $_POST['mdp'])
		{
			$contenu .= "<div class='erreur'>Mot de passe différents.</div>";
		}
		else
		{
			$lenKey = 8;
			$key = "";

			for ($i=0 ; $i<$lenKey ; $i++)
			{
				$key .= mt_rand (0, 9);
			}
			
			foreach($_POST as $indice => $valeur)
			{
				$_POST[$indice] = htmlEntities(addSlashes($valeur));
			}
			executeRequete("INSERT INTO client (civilite, nom, prenom, email, mdp, ville, code_postal, adresse, confirmatiokey) VALUES ('$_POST[civilite]', '$_POST[nom]', '$_POST[prenom]', '$_POST[email]', '$_POST[mdp]', '$_POST[ville]', '$_POST[code_postal]', '$_POST[adresse]', $key)");
			$contenu .= "<div class='validation'>Demande d'inscription bien enregistrer. </div>";
			$to = htmlspecialchars($_POST['email']);
	        $header="MIME-Version: 1.0\r\n";
	        $header.='From:monsitel2info@gmail.com'."\n";
	        $header.='Content-Type:text/html; charset="uft-8"'."\n";
	        $header.='Content-Transfer-Encoding: 8bit';
	        $message='
	        	<html>
	            	<body>
	                	<div align="center">
						Code de confirmation : '.$key.'<br>
						<a href=" localhost' . RACINE_SITE . '"confirmation.php?email='.urlencode($to).'&key='.$key.'">Cliquer ici pour confirmez votre compte !</a>
	                    </div>
	                </body>
	            </html>';
	            mail($to, "Confirmation de compte", $message, $header);

			header("location:connexion.php"); 
		}
	}
}
?>
<?php require_once("inc/haut.inc.php"); ?>
<?php echo $contenu; ?>

<p class="forms-titles">INSCRIPTION</p>
<form method="post" action="" class="forms">
	<label for="civilite"></label><br>
	<input name="civilite" value="m" checked="" type="radio">M
	<input name="civilite" value="f" type="radio">Mme<br><br>
		
	<label for="nom">Nom</label><br>
	<input type="text" id="nom" name="nom" placeholder="votre nom"><br><br>
		
	<label for="prenom">Prénom</label><br>
	<input type="text" id="prenom" name="prenom" placeholder="votre prénom"><br><br>

	<label for="email">Email</label><br>
	<input type="email" id="email" name="email" placeholder="exemple@gmail.com"><br><br>
		
	<label for="mdp">Mot de passe</label><br>
	<input type="password" id="mdp" name="mdp" minlength="8" required="required"><br><br>

	<label for="mdp_conf">Confirmation de mot de passe</label><br>
	<input type="password" id="mdp_conf" name="mdp_conf" required="required"><br><br>
				
	<label for="ville">Ville</label><br>
	<input type="text" id="ville" name="ville" placeholder="votre ville" pattern="[a-zA-Z0-9-_.]{5,15}" title="caractéres acceptés : a-zA-Z0-9-_."><br><br>
		
	<label for="cp">Code Postal</label><br>
	<input type="text" id="code_postal" name="code_postal" placeholder="code postal" pattern="[0-9]{5}" title="5 chiffres requis : 0-9"><br><br>
		
	<label for="adresse">Adresse</label><br>
	<textarea id="adresse" name="adresse" placeholder="votre dresse" pattern="[a-zA-Z0-9-_.]{5,15}" title="caractéres acceptés :  a-zA-Z0-9-_."></textarea><br><br>

	<input name="inscription" value="S'inscrire" type="submit">
	
</form>
 
<?php require_once("inc/bas.inc.php"); ?>