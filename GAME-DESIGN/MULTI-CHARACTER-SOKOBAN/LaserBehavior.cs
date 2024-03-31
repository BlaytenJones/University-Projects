using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserBehavior : MonoBehaviour
{
    private void OnTriggerEnter(Collider other) {
        if (GameManager.Instance.start && other.CompareTag("Player")) {
            other.transform.parent.GetComponent<PlayerBehavior>().GameOver();
        }
    }
}
