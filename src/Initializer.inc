<?php

declare(strict_types=1);

namespace RSSMerger;

use Monolog\Handler\ErrorLogHandler;
use Monolog\Logger;

/**
 * Class Initializer
 */
class Initializer
{
    /**
     * Initialize function.
     */
    public static function initialize() : void
    {
        self::logSetup();
    }


    /**
     * Logger Setup
     *
     * @return void;
     */
    private static function logSetup() : void
    {
        $logger = new Logger('rss-merger');

        $errorLogHandler = new ErrorLogHandler();
        $logger->pushHandler($errorLogHandler);

        $GLOBALS['log'] = $logger;
    }
}
