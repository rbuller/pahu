<?php
/*
Plugin Name: QR code
Plugin URI: http://blog.anthonywong.net/qr-code-wordpress-plugin/
Description: Creates and shows QR code images for blog articles.
Author: Anthony Wong
Author URI: http://anthonywong.net
Version: 0.5
*/ 

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

$qrcode_lib_path = get_option("qrcode_lib_path");
if (file_exists($qrcode_lib_path . "/qrcode_img.php")) {
	require_once($qrcode_lib_path . "/qrcode_img.php");
}

add_action('admin_menu', array('qrcode_wp_plugin', 'add_options_submenu'));

class qrcode_wp_plugin {

	function add_options_submenu() {
		if (!current_user_can('manage_options'))
			return;

		$minUserLevel = 1; // doesn't look like the function actually uses this
		                   // see code in admin-functions.php

		if (function_exists('add_options_page')) {
			add_options_page('QR code options', 'QR code', $minUserLevel,
			                 basename(__FILE__), array('qrcode_wp_plugin', 'admin_options_subpanel'));
		}
	}

	function admin_options_subpanel() {
		include_once('qrcode_options_subpanel.inc');
	}

	function get_qrcode_img_link() {
		global $id;
		$cache_dir = getcwd() . get_option("qrcode_image_path");
		$cache_file = $cache_dir . '/' . $id . '.png';

        // If the QR code image is not yet present, generate it.
		if (!file_exists($cache_file))
		{
			// echo '<!-- ' . qrcode_wp_plugin::construct_qrcode_data() . ' -->';
			qrcode_wp_plugin::generate_qrcode_image(qrcode_wp_plugin::construct_qrcode_data(), $cache_file);
		}

		$url = get_option("qrcode_image_path") . '/' . $id . '.png';
		echo '<img src="' . $url . '" ';
		$height = get_option("qrcode_image_height");
		if ($height != "") {
			echo 'width="' . $height . '" height="' . $height . '"';
		}
		echo '" />';
	}

	function construct_qrcode_data() {
		$data = '';
		$qrcode_encoded_data_options = get_option("qrcode_encoded_data_options");
		if (in_array("title", $qrcode_encoded_data_options)) {
			$data .= 'Title: ' . the_title('', '', false) . '; ';
		}
		if (in_array("posttime", $qrcode_encoded_data_options)) {
			$data .= 'Post time: ' . get_the_time('F j, Y') . ' ' .  get_the_time() . '; ';
		}
		if (in_array("url", $qrcode_encoded_data_options)) {
			if (count($qrcode_encoded_data_options) > 1) {
				$data .= 'URL: ';
			}
			$data .= apply_filters('the_permalink', get_permalink()) . '; ';
		}
		return substr($data, 0, strlen($data) - 2);
	}

	function get_test_qrcode_data() {
		$data = "";
		$qrcode_encoded_data_options = get_option("qrcode_encoded_data_options");
		if (in_array("title", $qrcode_encoded_data_options)) {
			$data .= 'Title: The quick brown fox jumped over the lazy dog; ';
		}
		if (in_array("posttime", $qrcode_encoded_data_options)) {
			$data .= 'Post time: ' . date('F j, Y H:i:s') . '; ';
		}
		if (in_array("url", $qrcode_encoded_data_options)) {
			if (count($qrcode_encoded_data_options) > 1) {
				$data .= 'URL: ';
			}
			$data .= apply_filters('the_permalink', get_permalink()) . '; ';
		}
		return substr($data, 0, strlen($data) - 2);
	}

	function get_test_qrcode_img_link($resize = 1) {
		$url = get_option("qrcode_image_path") . '/test.png';
		echo '<img src="' . $url . '" ';
		if ($resize == 1) {
			$height = get_option("qrcode_image_height");
			if ($height != "") {
				echo 'width="' . $height . '" height="' . $height . '"';
			}
		}
		echo '/>';
	}

/*
	function get_clean_test_qrcode_img_link() {
		$url = get_option("qrcode_image_path") . '/test.png';
		echo '<img src="' . $url . '" />';
	}
*/

	function generate_test_qrcode_img() {
		// Note that the current directory is wp-admin/
		$cache_dir = getcwd() . '/../' . get_option("qrcode_image_path");
		$cache_file = $cache_dir . '/test.png';
		$data = qrcode_wp_plugin::get_test_qrcode_data();

		qrcode_wp_plugin::generate_qrcode_image($data, $cache_file);
	}

	function generate_qrcode_image($qrcode_data, $cache_file) {
		$qr_image = new Qrcode_image;

		if (get_option("qrcode_version") != "0")
		{
			$qr_image->set_qrcode_version(get_option("qrcode_version"));
		}
		$qr_image->set_qrcode_error_correct(get_option("qrcode_ecc_level"));
		$qr_image->set_module_size(get_option("qrcode_module_size"));
		$qr_image->set_quietzone(5);

		$qr_image->qrcode_image_out($qrcode_data, "png", $cache_file); 
	}
}

?>
