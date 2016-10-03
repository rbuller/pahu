<?php

/*
 * Copyright (c) 2006, Anthony Wong
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

define("DEFAULT_IMAGE_PATH", "/wp-content/qrcode");
add_option("qrcode_lib_path", "");
add_option("qrcode_image_path", DEFAULT_IMAGE_PATH);
add_option("qrcode_encoded_data_options", array("title", "posttime", "url"));
add_option("qrcode_version", "0");
add_option("qrcode_ecc_level", "M");
add_option("qrcode_module_size", "5");
//add_option("qrcode_quietzone", "5");

// Process forms
if (isset($_POST["update_lib_path"])) {
	update_option("qrcode_lib_path", $_POST["qrcode_lib_path"]);
}

if (isset($_POST["update_image_path"])) {
	update_option("qrcode_image_path", $_POST["qrcode_image_path"]);
}

if (isset($_POST["update_options"])) {
	if (empty($_POST["qrcode_encoded_data_options"])) {
		echo '<div class="updated error">You must select at least one data to encode.</div>';
	} else {
		update_option("qrcode_encoded_data_options", $_POST["qrcode_encoded_data_options"]);
	}
	if (empty($_POST["qrcode_image_height"])) {
		update_option("qrcode_image_height", "");
	} else {
		update_option("qrcode_image_height", $_POST["qrcode_image_height"]);
	}
	qrcode_wp_plugin::generate_test_qrcode_img();
}

if (isset($_POST["update_params"])) {
	update_option("qrcode_version", $_POST["qrcode_version"]);
	update_option("qrcode_ecc_level", $_POST["qrcode_ecc_level"]);
	update_option("qrcode_module_size", $_POST["qrcode_module_size"]);
//	update_option("qrcode_quietzone", $_POST["qrcode_quietzone"]);
	qrcode_wp_plugin::generate_test_qrcode_img();
}

if (isset($_POST["re-generate"])) {
	qrcode_wp_plugin::generate_test_qrcode_img();
}

// Initialization
$qrcode_lib_path = get_option("qrcode_lib_path");
$qrcode_image_path = get_option("qrcode_image_path");
$qrcode_encoded_data_options = get_option("qrcode_encoded_data_options");
$qrcode_image_height = get_option("qrcode_image_height");
$qrcode_version = get_option("qrcode_version");
$qrcode_ecc_level = get_option("qrcode_ecc_level");
$qrcode_module_size = get_option("qrcode_module_size");
//$qrcode_quietzone = get_option("qrcode_quietzone");

if (empty($qrcode_image_path)) {
	$qrcode_image_path = DEFAULT_IMAGE_PATH;
}
if (empty($qrcode_version)) {
	$qrcode_version = "0";
}
if (empty($qrcode_ecc_level)) {
	$qrcode_ecc_level = "M";
}
if (empty($qrcode_module_size)) {
	$qrcode_module_size = "5";
}
//if (empty($qrcode_quietzone)) {
//	$qrcode_quietzone = "5";
//}

?>

<div class="wrap">
	<h2>QR code for Wordpress options</h2>

    <p>
	To make QR code works for your blog, there are three steps:
		<ol>
			<li>Download the QR code PHP library at <a href="http://www.venus.dti.ne.jp/~swe/program/qrcode_php0.50beta10.tar.gz">this site</a>, and extract it to where your web server can access it.</li>
			<li>Complete the options in this page, and make sure that the test image can be generated.</li>
			<li>Modify your theme to show the generated QR codes for each of your blog articles. This is a more
			    difficult step and you have to be familiar with HTML and PHP to do that. Open <code>index.php</code>
				and <code>single.php</code>, etc, and identify where you want to place the QR code image, then insert
				this PHP code snippet:<br/>
				<code>&lt;?php qrcode_wp_plugin::get_qrcode_img_link(); ?&gt;</code>
			</li>
		</ol>
	</p>

	<form method="post">
		<fieldset class="options">
			<legend>QR code library location</legend>
			<p>State where you have put the QR code library. If you have not yet downloaded the QR code
			   library, you can get it
			   <a href="http://www.venus.dti.ne.jp/~swe/program/qrcode_php0.50beta10.tar.gz">here</a>,
			   and then extract it to where the web server can access it. Make sure that you have entered
			   the path correctly, otherwise you will get "Call to undefined function" errors when generating
			   the image.
			</p>
			<table class="editform optiontable">
				<tr>
					<th width="33%" valign="top"><label for="qrcode_image_path">QR code library directory<br/>(absolute path):</label></th>
					<td>
						<input class="code" type="text" id="qrcode_lib_path" name="qrcode_lib_path" size="40" value="<?php echo $qrcode_lib_path; ?>" /><br />
						<?php if (strlen($qrcode_lib_path) > 0 && !file_exists($qrcode_lib_path)) { ?>
							<span style="color: red">"<?php echo $qrcode_lib_path ?>" not found.</span>
						<?php } ?>
					</td>
				</tr>
			</table>
		</fieldset>

		<p class="submit">
			<input name="update_lib_path" value="Update Path &raquo;" type="submit" />
		</p>
	</form>

	<form method="post">
		<fieldset class="options">
			<legend>Image files location</legend>
			<p>State where to store the generated image files. This directory is relative to your Wordpress
			   installation. You must create the directory on the server by yourself, the plugin will not
               do that for you. The directory must be writeable by the web server (use <code>chmod</code>,
               <code>chown</code> or <code>chgrp</code> to achieve that  if you are running Linux/Unix flavor).</p>
			<table class="editform optiontable">
				<tr>
					<th width="33%" valign="top"><label for="qrcode_image_path">Image files directory:</label></th>
					<td>
						<input class="code" type="text" id="qrcode_image_path" name="qrcode_image_path" size="40" value="<?php echo $qrcode_image_path; ?>" /><br />
						Recommended: <code>/wp-content/qrcode</code>
					</td>
				</tr>
			</table>
		</fieldset>

		<p class="submit">
			<input name="update_image_path" value="Update Path &raquo;" type="submit" />
		</p>
	</form>

	<form method="post">
		<fieldset class="options">
			<legend>Options</legend>
			<p>Define what data to be encoded in the QR code image:
			&nbsp;

			<input type="checkbox" name="qrcode_encoded_data_options[]" id="qrcode_encoded_data_url" value="url"<?php if(!empty($qrcode_encoded_data_options) && in_array("url", $qrcode_encoded_data_options)) echo ' checked="checked"'; ?> />
			URL
			&nbsp;
			<input type="checkbox" name="qrcode_encoded_data_options[]" id="qrcode_encoded_data_title" value="title"<?php if(!empty($qrcode_encoded_data_options) && in_array("title", $qrcode_encoded_data_options)) echo ' checked="checked"'; ?> />
			Title
			&nbsp;
			<input type="checkbox" name="qrcode_encoded_data_options[]" id="qrcode_encoded_data_posttime" value="posttime"<?php if(!empty($qrcode_encoded_data_options) && in_array("posttime", $qrcode_encoded_data_options)) echo ' checked="checked"'; ?> />
			Post time
			</p>

			<p>If you think the output image is too big or too small, you can resize it by putting the height of
			the image you wish below. As QR code is square-shaped, the height and width are the same, so you just
			need to supply the height.
			<p>
			<table class="editform optiontable">
				<tr>
					<th width="33%" valign="top"><label for="qrcode_image_height">Image height:</label></th>
					<td>
						<input class="code" type="text" id="qrcode_image_height" name="qrcode_image_height" size="5" value="<?php echo $qrcode_image_height ; ?>" /><br />
					</td>
				</tr>
			</table>
			
		</fieldset>

		<p class="submit">
			<input name="update_options" value="Update Options &raquo;" type="submit" />
		</p>
	</form>

	<form method="post">
		<fieldset class="options">
			<legend>QR code parameters</legend>
			<p>You can fine tune various parameters of the generated QR code images.</p>
			<table class="editform optiontable">
				<tr>
					<th width="33%" valign="top"><label for="qrcode_version">QR code version:</label></th>
					<td>
					  <select id="qrcode_version" name="qrcode_version">
							<option value="0" <?php echo $qrcode_version == "0" ? 'selected="selected"' : '' ?>>Auto</option>
							<option value="1" <?php echo $qrcode_version == "1" ? 'selected="selected"' : '' ?>>1</option>
							<option value="2" <?php echo $qrcode_version == "2" ? 'selected="selected"' : '' ?>>2</option>
							<option value="3" <?php echo $qrcode_version == "3" ? 'selected="selected"' : '' ?>>3</option>
							<option value="4" <?php echo $qrcode_version == "4" ? 'selected="selected"' : '' ?>>4</option>
							<option value="5" <?php echo $qrcode_version == "5" ? 'selected="selected"' : '' ?>>5</option>
							<option value="6" <?php echo $qrcode_version == "6" ? 'selected="selected"' : '' ?>>6</option>
							<option value="7" <?php echo $qrcode_version == "7" ? 'selected="selected"' : '' ?>>7</option>
							<option value="8" <?php echo $qrcode_version == "8" ? 'selected="selected"' : '' ?>>8</option>
							<option value="9" <?php echo $qrcode_version == "9" ? 'selected="selected"' : '' ?>>9</option>
							<option value="10" <?php echo $qrcode_version == "10" ? 'selected="selected"' : '' ?>>10</option>
							<option value="11" <?php echo $qrcode_version == "11" ? 'selected="selected"' : '' ?>>11</option>
							<option value="12" <?php echo $qrcode_version == "12" ? 'selected="selected"' : '' ?>>12</option>
							<option value="13" <?php echo $qrcode_version == "13" ? 'selected="selected"' : '' ?>>13</option>
							<option value="14" <?php echo $qrcode_version == "14" ? 'selected="selected"' : '' ?>>14</option>
							<option value="15" <?php echo $qrcode_version == "15" ? 'selected="selected"' : '' ?>>15</option>
							<option value="16" <?php echo $qrcode_version == "16" ? 'selected="selected"' : '' ?>>16</option>
							<option value="17" <?php echo $qrcode_version == "17" ? 'selected="selected"' : '' ?>>17</option>
							<option value="18" <?php echo $qrcode_version == "18" ? 'selected="selected"' : '' ?>>18</option>
							<option value="19" <?php echo $qrcode_version == "19" ? 'selected="selected"' : '' ?>>19</option>
							<option value="20" <?php echo $qrcode_version == "20" ? 'selected="selected"' : '' ?>>20</option>
						</select>
						(Default: <code>Auto</code>)
					</td>
				</tr>
				<tr>
					<th width="33%" valign="top"><label for="qrcode_ecc_level">ECC level:</label></th>
					<td>
					  <select id="qrcode_ecc_level" name="qrcode_ecc_level">
							<option value="L" <?php echo $qrcode_ecc_level == "L" ? 'selected="selected"' : '' ?>>L</option>
							<option value="M" <?php echo $qrcode_ecc_level == "M" ? 'selected="selected"' : '' ?>>M</option>
							<option value="Q" <?php echo $qrcode_ecc_level == "Q" ? 'selected="selected"' : '' ?>>Q</option>
							<option value="H" <?php echo $qrcode_ecc_level == "H" ? 'selected="selected"' : '' ?>>H</option>
						</select>
						(Default: <code>M</code>)
					</td>
				</tr>
				<tr>
					<th width="33%" valign="top"><label for="qrcode_module_size">Module size:</label></th>
					<td>
					  <select id="qrcode_module_size" name="qrcode_module_size">
							<option value="1" <?php echo $qrcode_module_size == "1" ? 'selected="selected"' : '' ?>>1</option>
							<option value="2" <?php echo $qrcode_module_size == "2" ? 'selected="selected"' : '' ?>>2</option>
							<option value="3" <?php echo $qrcode_module_size == "3" ? 'selected="selected"' : '' ?>>3</option>
							<option value="4" <?php echo $qrcode_module_size == "4" ? 'selected="selected"' : '' ?>>4</option>
							<option value="5" <?php echo $qrcode_module_size == "5" ? 'selected="selected"' : '' ?>>5</option>
							<option value="6" <?php echo $qrcode_module_size == "6" ? 'selected="selected"' : '' ?>>6</option>
							<option value="7" <?php echo $qrcode_module_size == "7" ? 'selected="selected"' : '' ?>>7</option>
							<option value="8" <?php echo $qrcode_module_size == "8" ? 'selected="selected"' : '' ?>>8</option>
						</select>
						(Default: <code>3</code>)
					</td>
				</tr>
<?php
/*
				<tr>
					<th width="33%" valign="top"><label for="qrcode_quietzone">Quiet zone:</label></th>
					<td>
					  <select id="qrcode_quietzone" name="qrcode_quietzone">
							<option value="1" <?php echo $qrcode_quietzone == "1" ? 'selected="selected"' : '' ?>>1</option>
							<option value="2" <?php echo $qrcode_quietzone == "2" ? 'selected="selected"' : '' ?>>2</option>
							<option value="3" <?php echo $qrcode_quietzone == "3" ? 'selected="selected"' : '' ?>>3</option>
							<option value="4" <?php echo $qrcode_quietzone == "4" ? 'selected="selected"' : '' ?>>4</option>
							<option value="5" <?php echo $qrcode_quietzone == "5" ? 'selected="selected"' : '' ?>>5</option>
							<option value="6" <?php echo $qrcode_quietzone == "6" ? 'selected="selected"' : '' ?>>6</option>
							<option value="7" <?php echo $qrcode_quietzone == "7" ? 'selected="selected"' : '' ?>>7</option>
							<option value="8" <?php echo $qrcode_quietzone == "8" ? 'selected="selected"' : '' ?>>8</option>
						</select>
						(Default: <code>5</code>)
					</td>
				</tr>
*/ ?>
			</table>
		</fieldset>

		<p class="submit">
			<input name="update_params" value="Update Parameters &raquo;" type="submit" />
		</p>
	</form>

	<form method="post">
		<fieldset class="options">
			<legend>Test Image</legend>
			<p>Press the "Re-generate Test Image" button to re-generate a sample QR code image by using the above settings.</p>
			<div>
				<div style="float: left; padding: 5px; margin: 5px; border: 1px solid #ccc; text-align: center;">
					<?php qrcode_wp_plugin::get_test_qrcode_img_link(); ?>
					<br/>
					<b>Sample image (resized)</b>
				</div>
				<div style="float: left; padding: 5px; margin: 5px; border: 1px solid #ccc; text-align: center;">
					<?php qrcode_wp_plugin::get_test_qrcode_img_link(0); ?>
					<br/>
					<b>Sample image (original size)</b>
				</div>
			</div>
			<div style="clear: both">
				<p><b>Data in the image:</b> (testing data)<br/>
					<?php echo qrcode_wp_plugin::get_test_qrcode_data(); ?>
				</p>
				<p class="submit">
					<input name="re-generate" value="Re-generate Test Image &raquo;" type="submit" />
				</p>
			</div>
		</fieldset>
	</form>
</div>
