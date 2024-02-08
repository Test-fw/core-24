<?php

/*
 * Copyright (C) 2023 Deciso B.V.
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

function kea_services()
{

<<<<<<< HEAD:src/etc/inc/plugins.inc.d/squid.inc
    $services = array();

    if (
        isset($config['Reticen8']['proxy']['general']['enabled']) &&
        $config['Reticen8']['proxy']['general']['enabled'] == 1
    ) {
        $services[] = array(
            'description' => gettext('Squid Web Proxy'),
            'configd' => array(
                'restart' => array('proxy restart'),
                'start' => array('proxy start'),
                'stop' => array('proxy stop'),
            ),
            'pidfile' => '/var/run/squid/squid.pid',
            'name' => 'squid',
        );
=======
    $services = [];
    if (!empty((string)(new \Reticen8\Kea\KeaDhcpv4())->general->enabled)) {
        $services[] = [
            'description' => gettext('KEA DHCPv4 server'),
            'pidfile' => '/var/run/kea/kea-dhcp4.kea-dhcp4.pid',
            'configd' => [
                'restart' => ['kea restart'],
                'start' => ['kea start'],
                'stop' => ['kea stop'],
            ],
            'name' => 'kea-dhcpv4',
        ];
>>>>>>> 98878a9eb90c1150b232bfbc7e9a012a3e3462a0:src/etc/inc/plugins.inc.d/kea.inc
    }
    return $services;
}


<<<<<<< HEAD:src/etc/inc/plugins.inc.d/squid.inc
    $result[] = array(
        'description' => gettext('Squid Web Proxy'),
        'section' => 'Reticen8.proxy',
        'id' => 'squid',
        'services' => ["squid"],
    );
=======
function kea_syslog()
{
    $logfacilities = [];
    $logfacilities['kea'] = ['facility' => ['kea-dhcp4', 'kea-dhcp6', 'kea-ctrl-agent']];
    return $logfacilities;
}


function kea_xmlrpc_sync()
{
    $result = [];

    $result[] = [
        'description' => gettext('Kea DHCP'),
        'section' => 'Reticen8.Kea',
        'id' => 'kea',
        'services' => ["kea-dhcpv4"],
    ];
>>>>>>> 98878a9eb90c1150b232bfbc7e9a012a3e3462a0:src/etc/inc/plugins.inc.d/kea.inc

    return $result;
}
