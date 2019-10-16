<?php

namespace RSSMerger;

use Error;

/**
 * Class BaseError
 */
class BaseError
{

    /**
     * Error type.
     *
     * @var Error|null error
     */
    public Error $error;


    /**
     * Constructor
     *
     * @param Error|null $error Error.
     */
    public function __construct(?Error $error=null)
    {
        $this->error = $error;
        $this->logError();

    }//end __construct()


    /**
     * Render Error page to HTML
     *
     * @param integer     $code    HTTPCode.
     * @param string|null $message Custom message.
     *
     * @return void
     */
    public function renderHTML(int $code=500, string $message=null) : void
    {
        http_response_code($code);
        $renderer = include 'template/error.php';
        echo $renderer(
            [
                'code'    => $code,
                'message' => $this->readMessage($message),
            ]
        );

    }//end renderHTML()


    /**
     * Render Error page to XML
     *
     * @param integer     $code    HTTPCode.
     * @param string|null $message Custom message.
     *
     * @return void
     */
    public function renderXML(int $code=500, string $message=null) : void
    {
        http_response_code($code);
        header('Content-Type: application/xml; charset=UTF-8');
        $renderer = include 'template/xmlerror.php';
        echo $renderer(
            [
                'code'    => $code,
                'message' => $this->readMessage($message),
            ]
        );

    }//end renderXML()


    /**
     * Read message from various source.
     *
     * @param string|null $message A custom message.
     *
     * @return string
     */
    private function readMessage(?string $message) : string
    {
        if ($message !== null) {
            return $message;
        } else if ($this->error !== null) {
             return $this->error->getMessage();
        } else {
            return 'Something went wrong';
        }

    }//end readMessage()


    /**
     * Log error to logger.
     *
     * @return void
     */
    private function logError() : void
    {
        $GLOBALS['log']->error($this->error);

    }//end logError()


}//end class
