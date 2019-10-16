<?php

namespace RSSMerger;

class StaticValue
{
    protected static $GENERATOR_NAME = "PHP RSS Merger";
    protected static $GENERATOR_VERSION = "0.0.1-dev";

    public static function GENERATOR()
    {
        return self::$GENERATOR_NAME . " " . self::$GENERATOR_VERSION;
    }
}
