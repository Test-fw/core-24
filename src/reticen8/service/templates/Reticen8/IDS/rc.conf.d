{% if not helpers.empty('Reticen8.IDS.general.enabled') %}
suricata_setup="/usr/local/reticen8/scripts/suricata/setup.sh"
suricata_enable="YES"
{% if not helpers.empty('Reticen8.IDS.general.verbosity') %}
suricata_flags="-D -{{Reticen8.IDS.general.verbosity}}"
{% endif %}
{% if Reticen8.IDS.general.ips|default("0") == "1" %}
# IPS mode, switch to netmap
suricata_netmap="YES"
{% else %}
# IDS mode, pcap live mode
{% set addFlags=[] %}
{%   for intfName in Reticen8.IDS.general.interfaces.split(',') %}
{#     store additional interfaces to addFlags #}
{%     do addFlags.append(helpers.physical_interface(intfName)) %}
{%   endfor %}
suricata_interface="{{ addFlags|join(' ') }}"
{% endif %}
{% else %}
suricata_enable="NO"
{% endif %}
