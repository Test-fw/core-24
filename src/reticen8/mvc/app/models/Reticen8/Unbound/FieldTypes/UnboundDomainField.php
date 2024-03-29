<?php

/**
 *    Copyright (C) 2022 Deciso B.V.
 *
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *    THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *    AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 *
 */

namespace Reticen8\Unbound\FieldTypes;

use Reticen8\Base\FieldTypes\BaseField;
use Reticen8\Base\Validators\CallbackValidator;
use Reticen8\Firewall\Util;

/**
 * Class UnboundDomainField
 * @package Reticen8\Unbound\FieldTypes
 */
class UnboundDomainField extends BaseField
{
    protected $internalIsContainer = false;

    public function getValidators()
    {
        /**
         * XXX: the only reason this class exists is to be compatible with
         * legacy code allowing '_msdcs' domain prefixes. This is weird, not
         * conforming to any standard and should probably be looked into further.
         */
        $validators = parent::getValidators();
        if ($this->internalValue != null) {
            $validators[] = new CallbackValidator([
                "callback" => function ($data) {
                    if (substr($data, 0, 6) == '_msdcs') {
                        $subdomain = substr($data, 7);
                        if (substr($data, 6, 1) != '.' || !Util::isDomain($subdomain)) {
                            return [gettext("A valid domain must be specified after '_msdcs', prefixed with a dot")];
                        }
                    } elseif (!Util::isDomain($data)) {
                        return [gettext("A valid domain must be specified")];
                    }
                    return [];
                }
            ]);
        }

        return $validators;
    }
}
