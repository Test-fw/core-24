#!/usr/local/bin/php
<?php

/*
 * Copyright (C) 2021 Deciso B.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

require_once("auth.inc");
require_once("config.inc");

$opts = getopt('hu:o', array(), $optind);
$args = array_slice($argv, $optind);

if (isset($opts['h']) || empty($opts['u'])) {
    echo "Usage: add_user.php [-h] \n";
    echo "\t-h show this help text and exit\n";
    echo "\t-u [required] username\n";
    echo "\t-o origin (default=automation)";
    exit(-1);
} else {
    Reticen8\Core\Config::getInstance()->lock();
    $config = Reticen8\Core\Config::getInstance()->toArray(listtags());
    $a_user = &config_read_array('system', 'user');
    $input_errors = [];
    if (preg_match("/[^a-zA-Z0-9\.\-_]/", $opts['u'])) {
        $input_errors[] = gettext("The username contains invalid characters.");
    } elseif (strlen($opts['u']) > 32) {
        $input_errors[] = gettext("The username is longer than 32 characters.");
    } else {
        foreach ($a_user as $userent) {
            if ($userent['name'] == $opts['u']) {
                $input_errors[] = gettext("Another entry with the same username already exists.");
                break;
            }
        }
    }

    if (empty($input_errors)) {
        $userent = [];
        $userent['uid'] = $config['system']['nextuid']++;
        $userent['name'] = $opts['u'];
        $userent['descr'] = "";
        $userent['scope'] = !empty($opts['o']) ? $opts['o'] : 'automation';
        local_user_set_password($userent);
        local_user_set($userent);
        $a_user[] = $userent;
        write_config(sprintf("user \"%s\" created", $userent['name']));
        // XXX: signal backend that the user has changed.
        configdp_run('auth user changed', [$userent['name']]);
        echo json_encode(["status" => "ok", "uid" => $userent['uid'], "name" => $userent['name']]);
    } else {
        echo json_encode(["status" => "failed", "messages" => $input_errors]);
    }
}
