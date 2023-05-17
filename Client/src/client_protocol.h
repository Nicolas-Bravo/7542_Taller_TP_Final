#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <string>
#include <sstream>
#include "common_socket.h"
#include "common_responsedto.h"
#include "common_commanddto.h"
/*
 * TDA ClientProtocol
 * Se comunica con el servidor a través de su socket.
 */
class ClientProtocol{
    private:
        Socket skt;

        /*
         * Método privado para indicar al servidor que se quiere
         * crear una partida. Encapsula la respuesta del servior
         * en un objeto ResponseDTO.
         */
        ResponseDTO create(const std::string& scenario);

        /*
         * Método privado para indicar al servidor que se quiere
         * unir a una partida. Encapsula la respuesta del servior
         * en un objeto ResponseDTO.
         */
        ResponseDTO join(const uint32_t& code);

        /*
         * Método privado para leer del servidor n mensajes.
         * Encapsula la respuesta del servior en un objeto ResponseDTO.
         */
        ResponseDTO readMsg(const uint32_t& n);

        /*
         * Método privado para enviar al servidor un mensajes.
         * Encapsula la respuesta del servior en un objeto ResponseDTO.
         */
        ResponseDTO sendMsg(const std::string& str);

        /*
         * Método privado para enviar los bytes correspondientes
         * por medio del socket.
         */
        void sendAll(
            const void *data,
            unsigned int sz);

        /*
         * Método privado para recibir los bytes correspondientes
         * por medio del socket.
         */
         void recvAll(
             void *data,
             unsigned int sz);

         /*
          * Método privado para enviar strings por medio del socket.
          */
          void sendString(const std::string& str);

          /*
           * Método privado para recibir strings
           */
          std::string recvString();

    public:
        /*
        * Constructor para conectarse a un servidor
        * (`ClientProtocol::ClientProtocol(const char*, const char*)`)
        * */
        explicit ClientProtocol(
            const std::string& hostname,
            const std::string& servname);

        /*
         * Método generico para indicar al servidor que se ingresó
         * un comando:
         */
        ResponseDTO sendCommand(const CommandDTO& commanddto);

        /*
        * No queremos permitir que alguien haga copias
        * */
        ClientProtocol(const ClientProtocol&) = delete;
        ClientProtocol& operator=(const ClientProtocol&) = delete;

        /*
        * Queremos permitir mover a los objetos (move semantics).
        *
        * Como todos nuestros atributos son movibles, la implementación
        * por default de C++ nos alcanza.
        * */
        ClientProtocol(ClientProtocol&&) = default;
        ClientProtocol& operator=(ClientProtocol&&) = default;
};
#endif  // CLIENT_PROTOCOL_H_
