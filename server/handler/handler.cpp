#include "handler.h"
#include "ihandler.h"
#include "chathandler.h"
#include "loginhandler.h"
#include "roomhandler.h"
#include "registerhandler.h"
#include "gifthandler.h"

Handler::Handler()
{

}

void Handler::handle(QTcpSocket *socket, const Protocol &protocol)
{
    IHandler* handler = nullptr;
    switch (protocol.getType()) {
    case Protocol::gift:
        handler = new GiftHandler;
        break;
    case Protocol::chat:
        handler = new ChatHandler;
        break;
    case Protocol::room:
        handler = new RoomHandler;
        break;
    case Protocol::login:
        handler = new LoginHandler;
        break;
    case Protocol::regist:
        handler = new RegisterHandler;
        break;
    case Protocol::none:
        break;
    default:
        break;
    }

    if(handler != nullptr){
        handler->handle(socket,protocol);
        delete handler;
    }
}

