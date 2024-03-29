<?php

/*
 * Copyright (C) 2022 Deciso B.V.
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

namespace Reticen8\Unbound\FieldTypes;

use Reticen8\Base\FieldTypes\BaseField;
use Reticen8\Base\Validators\CallbackValidator;
use Reticen8\Firewall\Util;

/**
 * Class UnboundServerField
 * @package Reticen8\Unbound\FieldTypes
 */
class UnboundServerField extends BaseField
{
    /**
     * {@inheritdoc}
     */
    protected $internalIsContainer = false;

    /**
     * {@inheritdoc}
     */
    public function getValidators()
    {
        $validators = parent::getValidators();
        if ($this->internalValue != null) {
            $validators[] = new CallbackValidator([
                "callback" => function ($value) {
                    $parts = explode("@", $value);
                    if (count($parts) == 2 && (!Util::isIpAddress($parts[0]) || !Util::isPort($parts[1]))) {
                        return [gettext("A valid IP address and port must be specified, for example 192.168.100.10@5353.")];
                    } elseif (count($parts) != 2 && !Util::isIpAddress($value)) {
                        return [gettext("A valid IP address must be specified, for example 192.168.100.10.")];
                    }
                    return [];
                }
            ]);
        }
        return $validators;
    }
}
