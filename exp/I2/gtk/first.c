#include <gtk/gtk.h>
 void hello( GtkWidget *widget,
             gpointer   data )
 {
     printf ("Hello World\n");
 }
 int delete_event( GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   data )
 {
     printf ("delete event occurred\n");
     return 0;
 }
 void destroy( GtkWidget *widget,
               gpointer   data )
 {
   gtk_main_quit ();
 }
 int main( int   argc, char *argv[] )
 {
     GtkWidget *window;
     GtkWidget *button;
     gtk_init (&argc, &argv);
     /*ウィンドウを作る*/
     window = gtk_window_new (GTK_WINDOW_TOPLEVEL); // 新しいウィンドウを生成する
     gtk_container_set_border_width (GTK_CONTAINER (window), 10);
     // ウィンドウの幅
     gtk_widget_show (window); //ウィンドウを表示
     /*ボタンを作る*/
     button = gtk_button_new_with_label ("Hello"); //ボタンに''Hello''を書く
     gtk_container_add (GTK_CONTAINER (window), button); //ボタンをパックする
     gtk_widget_show (button); //ボタンを表示
     /*イベントに対応するコールバック関数*/
     g_signal_connect (G_OBJECT (window), "delete_event",
                        G_CALLBACK (delete_event), NULL); //終了用シグナル
     g_signal_connect (G_OBJECT (window), "destroy",
                        G_CALLBACK (destroy), NULL); //終了用シグナル
     g_signal_connect_swapped (G_OBJECT (button), "clicked",
                                G_CALLBACK (gtk_widget_destroy),
                                G_OBJECT (window));
                                //ウィンドウを閉じるボタンをクリックした時
     g_signal_connect (G_OBJECT (button), "clicked",
                        G_CALLBACK (hello), NULL);
                        //''Hello''ボタンをクリックした時
     gtk_main ();
     return 0;
 } 
